#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "regauditdialog.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QPushButton>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDir>
#include <QFile>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QTimer>
#include <QEvent>

class HoverMarqueeFilter : public QObject {
public:
    HoverMarqueeFilter(QObject *parent = nullptr) : QObject(parent) {}
protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        QPushButton *btn = qobject_cast<QPushButton*>(watched);
        if (btn) {
            if (event->type() == QEvent::Enter) {
                // start marquee on hover
                if (btn->property("originalText").isNull()) {
                    btn->setProperty("originalText", btn->text());
                    btn->setProperty("scrollPos", 0);
                    btn->setProperty("scrollText", btn->text() + "          ***          ");
                }
                QTimer *timer = btn->findChild<QTimer*>("marqueeTimer");
                if (!timer) {
                    timer = new QTimer(btn);
                    timer->setObjectName("marqueeTimer");
                    connect(timer, &QTimer::timeout, btn, [btn]() {
                        QString scrollText = btn->property("scrollText").toString();
                        if (scrollText.isEmpty()) return;
                        int pos = btn->property("scrollPos").toInt();
                        pos = (pos + 1) % scrollText.length();
                        btn->setProperty("scrollPos", pos);
                        QString display = scrollText.mid(pos) + scrollText.left(pos);
                        btn->setText(display);
                    });
                }
                // Only start scrolling if text is likely too long (e.g. > 30 chars), optionally we just scroll all.
                timer->start(350); // Marquee speed 
            } else if (event->type() == QEvent::Leave) {
                QTimer *timer = btn->findChild<QTimer*>("marqueeTimer");
                if (timer) timer->stop();
                if (!btn->property("originalText").isNull()) {
                    btn->setText(btn->property("originalText").toString());
                }
            }
        }
        return QObject::eventFilter(watched, event);
    }
};

static HoverMarqueeFilter *globalMarqueeFilter = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //初始化人脸识别引擎对象
    static ModelSetting FD_model("D:/D2/OpenCv/SeetaFace/bin/model/fd_2_00.dat",SEETA_DEVICE_AUTO);
    static ModelSetting PD_model("D:/D2/OpenCv/SeetaFace/bin/model/pd_2_00_pts5.dat",SEETA_DEVICE_AUTO);
    static ModelSetting FR_model("D:/D2/OpenCv/SeetaFace/bin/model/fr_2_10.dat",SEETA_DEVICE_AUTO);
    engin=new FaceEngine(FD_model, PD_model, FR_model);



    connect(&myManager,SIGNAL(finished(QNetworkReply *)),this,SLOT(fun(QNetworkReply *)));
    this->setWindowTitle("强强科技注册后台服务器");

    tcpserver = new QTcpServer(this);
    networkManager = new QNetworkAccessManager(this); // 初始化网络管理器

    // 监听任意IPv4地址，避免硬编码IP导致换电脑后无法启动
    if (!tcpserver->listen(QHostAddress::AnyIPv4, 20000)) {
        qDebug() << "服务器启动失败！";
    } else {
        qDebug() << "服务器启动成功，监听端口 20000";
    }
    
    connect(tcpserver, &QTcpServer::newConnection, this, &MainWindow::newClientLink);

    // 1. 设置滑动模式为按像素平滑滚动（鼠标滚轮滑起来更顺滑不卡顿）
    ui->infList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    // 2. 使用 QSS 美化滚动条：平时透明度低，鼠标悬浮时颜色加深，去掉难看的上下箭头
    QString modernScrollStyle = R"(
                                QListWidget {
                                outline: 0px;
                                border: 1px solid #ddd;
                                border-radius: 4px;
                                }
                                QScrollBar:vertical {
                                border: none;
                                background: transparent;
                                width: 8px;
                                margin: 0px;
                                }
                                QScrollBar::handle:vertical {
                                background: rgba(0, 0, 0, 0.2);
                                min-height: 30px;
                                border-radius: 4px;
                                }
                                QScrollBar::handle:vertical:hover {
                                background: rgba(0, 0, 0, 0.5); /* 鼠标悬停时颜色变深 */
                                }
                                QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                                height: 0px;
                                }
                                QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
                                background: transparent;
                                }
                                )";
    ui->infList->setStyleSheet(modernScrollStyle);
    ui->listWidget->setStyleSheet(modernScrollStyle);

    // 启动时加载现有的员工列表
    loadEmployeeList();
}

MainWindow::~MainWindow()
{
    tcpserver->close();
    delete ui;
}

// 加载和刷新员工列表的函数
void MainWindow::loadEmployeeList()
{
    // 清空现有的列表项，避免重复添加
    ui->listWidget->clear();

    // 尝试连接数据库
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
    }
    database.setDatabaseName("D:/share/Face_Recognition_Attendance_System/employeeinformation.db");

    if (!database.open()) {
        qDebug() << "刷新列表失败：无法打开数据库" << database.lastError().text();
        return;
    }

    // 查询所有员工数据 (查询字段需匹配建表结构 name, phone 作为电话, idcar)
    QSqlQuery query(database);
    QString sql = "SELECT name, phone, idcar, photoPath FROM employeetable";
    engin->Clear();
    if (query.exec(sql)) {
        while (query.next()) {
            QString nameStr = query.value(0).toString();
            QString phoneStr = query.value(1).toString(); // 数据库的"phone"存的是电话号码
            QString idcarStr = query.value(2).toString();
            // QString photoStr = query.value(3).toString();

            // 拼接要在列表里展示的格式
            QString employeeInformation = QString("姓名:%1  电话:%2  身份证:%3")
                    .arg(nameStr)
                    .arg(phoneStr)
                    .arg(idcarStr);
            ui->listWidget->addItem(employeeInformation);


            // 取出路径
            QString imgPath = query.value(3).toString();

            // 用OpenCV读取图片
            cv::Mat mat = cv::imread(imgPath.toLocal8Bit().data());
            if (mat.empty()) {
                qDebug() << "图片不存在：" << imgPath;
                continue;
            }

            // 转SeetaImageData（深拷贝，不崩溃）
            SeetaImageData data;
            data.width = mat.cols;
            data.height = mat.rows;
            data.channels = mat.channels();
            data.data = new unsigned char[data.width * data.height * data.channels];
            memcpy(data.data, mat.data, data.width * data.height * data.channels);

            // 注册到人脸引擎
            int64_t id = engin->Register(data);
            
            // 把映射存进内存，方便查询
            empNameMap[id] = nameStr;
            empPhoneMap[id] = phoneStr;

            // 释放内存
            delete[] data.data;
            qDebug()<<"注册人脸库 返回ID:"<<id<<" 姓名:"<<nameStr;

        }
        qDebug() << "员工列表刷新成功！";
    }
    else
    {
        qDebug() << "查询员工列表失败:" << query.lastError().text();
    }


}

void MainWindow::newClientLink()
{
    QTcpSocket *sock = tcpserver->nextPendingConnection();
    clientList.append(sock);

    QString ip = sock->peerAddress().toString();
    quint16 port = sock->peerPort();
    qDebug() << "新客户端连接:" << ip << port;

    connect(sock, &QTcpSocket::readyRead, this, &MainWindow::recvClientMsg);

    // 客户端断开
    connect(sock, &QTcpSocket::disconnected, this, [=]() {
        clientList.removeOne(sock);
        bufferMap.remove(sock); // 清理该客户端的缓存残余，防止内存泄漏
        sock->deleteLater();
        qDebug() << "客户端断开:" << ip << port;
    });
}

void MainWindow::recvClientMsg()
{
    QTcpSocket *sock = qobject_cast<QTcpSocket*>(sender());
    if (!sock) return;

    // 1. 接收到的网络流追加到该客户端专属的缓冲区 (解决半包/拆包问题)
    bufferMap[sock].append(sock->readAll());

    // 2. 按照分隔符 '\n' 循环解粘包。如果找不到 '\n'，说明一张大图片还没传完，此函数会直接返回，等下次网络包来继续拼
    while (bufferMap[sock].contains('\n'))
    {
        int index = bufferMap[sock].indexOf('\n');
        QByteArray packet = bufferMap[sock].left(index); // 提取出完整的一包
        bufferMap[sock].remove(0, index + 1);            // 删掉取完的数据以及 \n

        QString str = QString::fromUtf8(packet);
        QStringList msglist = str.split("@");

        // 格式检查，防崩溃
        if (msglist.isEmpty()) continue;

        //接收到打卡信息
        if(msglist.at(0)=="clockin" && msglist.size() >= 2)
        {
            // 1. 获取收到的 Base64 图片数据
            QString base64Image = msglist.at(1);

            // --------------------------
            // 关键：Base64 直接转 OpenCV 的 Mat（不存文件）
            // --------------------------
            QByteArray imgBytes = QByteArray::fromBase64(base64Image.toUtf8());
            QImage img;
            img.loadFromData(imgBytes);

            // QImage 转 cv::Mat
            cv::Mat destData;
            if (img.format() == QImage::Format_RGB888) {
                destData = cv::Mat(img.height(), img.width(), CV_8UC3, (uchar*)img.bits(), img.bytesPerLine());
                cv::cvtColor(destData, destData, cv::COLOR_RGB2BGR);
            } else {
                destData = cv::Mat(img.height(), img.width(), CV_8UC4, (uchar*)img.bits(), img.bytesPerLine());
                cv::cvtColor(destData, destData, cv::COLOR_BGRA2BGR);
            }

            // 判断图片是否有效
            if (destData.empty()) {
                qDebug() << "Base64 转图片失败！";
                return;
            }

            // 2. 转 SeetaFace 格式（深拷贝，不崩溃）
            SeetaImageData seetaData;
            seetaData.width = destData.cols;
            seetaData.height = destData.rows;
            seetaData.channels = destData.channels();

            // 必须深拷贝！！！不能直接 = destData.data
            seetaData.data = new uint8_t[seetaData.width * seetaData.height * seetaData.channels];
            memcpy(seetaData.data, destData.data, seetaData.width * seetaData.height * seetaData.channels);

            // 3. 执行人脸识别
            int64_t index;
            float similarityopencv;
            int ret = engin->QueryTop(seetaData, 1, &index, &similarityopencv);

            // 用完立即释放内存
            delete[] seetaData.data;

            // 4. 识别结果
            if (ret < 0) {
                qDebug() << "识别失败";
                sock->write("clockin@NO\n");
            } else {
                qDebug() << "识别结果！相似度：" << similarityopencv << " ID：" << index;
                if (similarityopencv > 0.75) {
                    qDebug() << "打卡成功，计算考勤状态";
                    
                    QDateTime now = QDateTime::currentDateTime();
                    QString currentDate = now.toString("yyyy-MM-dd");
                    QTime currentTime = now.time();
                    QString empId = QString::number(index);
                    
                    QSqlDatabase database;
                    if (QSqlDatabase::contains("qt_sql_default_connection")) {
                        database = QSqlDatabase::database("qt_sql_default_connection");
                    } else {
                        database = QSqlDatabase::addDatabase("QSQLITE");
                    }
                    //设置数据库路径
                    database.setDatabaseName("D:/share/Face_Recognition_Attendance_System/employeeinformation.db");
                    
                    if (database.open()) {
                        QSqlQuery query(database);
                        //创建员工打卡记录表
                        query.exec("create table if not exists attendancetable (empid text, date text, intime text, outtime text, status text);");
                        // 尝试添加 name 和 phone 字段（兼容以前建好的表）
                        query.exec("alter table attendancetable add column name text;");
                        query.exec("alter table attendancetable add column phone text;");

                        // 查询今天该员工是否打过卡
                        query.prepare("select intime, outtime from attendancetable where empid = :empid and date = :date");
                        query.bindValue(":empid", empId);
                        query.bindValue(":date", currentDate);
                        query.exec();

                        QString statusInfo = "";
                        if (query.next()) {
                            // 今天已存在记录 => 本次算作下班打卡
                            query.prepare("update attendancetable set outtime = :outtime, name = :name, phone = :phone where empid = :empid and date = :date");
                            query.bindValue(":outtime", currentTime.toString("hh:mm:ss"));
                            query.bindValue(":name", empNameMap.value(index, "Unknown"));
                            query.bindValue(":phone", empPhoneMap.value(index, "Unknown"));
                            query.bindValue(":empid", empId);
                            query.bindValue(":date", currentDate);
                            query.exec();

                            // 早退判断（假如规定18:00下班，为了测试早退效果也可以设其他时间，这里设为18:00标准）
                            if (currentTime < QTime(18, 0, 0)) {
                                statusInfo = "（下班）早退打卡成功！打卡时间：" + currentTime.toString("hh:mm:ss");
                            } else {
                                statusInfo = "下班打卡正常！打卡时间：" + currentTime.toString("hh:mm:ss");
                            }
                        } else {
                            // 今天不存在记录 => 上班打卡
                            query.prepare("insert into attendancetable (empid, name, phone, date, intime, outtime) values (:empid, :name, :phone, :date, :intime, '')");
                            query.bindValue(":empid", empId);
                            query.bindValue(":name", empNameMap.value(index, "Unknown"));
                            query.bindValue(":phone", empPhoneMap.value(index, "Unknown"));
                            query.bindValue(":date", currentDate);
                            query.bindValue(":intime", currentTime.toString("hh:mm:ss"));
                            query.exec();
                            
                            // 迟到判断（假如规定09:00上班）
                            if (currentTime > QTime(9, 0, 0)) {
                                statusInfo = "（上班）迟到打卡成功！打卡时间：" + currentTime.toString("hh:mm:ss");
                            } else {
                                statusInfo = "上班打卡正常！打卡时间：" + currentTime.toString("hh:mm:ss");
                            }
                        }
                        
                        QString msg = QString("clockin@ok@%1\n").arg(statusInfo);
                        sock->write(msg.toUtf8());
                    } else {
                        sock->write("clockin@ok@打卡成功（无考勤状态）\n");
                    }
                } else {
                    qDebug() << "相似度过低，打卡失败";
                    sock->write("clockin@no\n"); // 修复拼写错误，原为 clock@no
                }
            }
            sock->flush();
        }

        //接收到注册信息
        // 这里判断 size >= 5 是因为有了 Base64：reg@姓名@电话@身份证@Base64长字符串
        if (msglist.at(0) == "reg" && msglist.size() >= 5)
        {
            QString nameStr = msglist.at(1);
            QString phoneStr = msglist.at(2);
            QString idcarStr = msglist.at(3);
            QString base64Image = msglist.at(4);

            QString ip = sock->peerAddress().toString();
            if (ip.startsWith("::ffff:")) ip = ip.mid(7);
            QString port = QString::number(sock->peerPort());

            // --- 解决图片保存需求 ---
            QString dirPath = "D:/share/Face_Recognition_Attendance_System/faceServer/image";
            QDir dir;
            if (!dir.exists(dirPath)) {
                dir.mkpath(dirPath); // 自动创建不存在的路径
            }

            // 使用“姓名_电话.jpg”作为文件名保存以防止重名覆盖
            QString imgPath = QString("%1/%2_%3.jpg").arg(dirPath).arg(nameStr).arg(phoneStr);
            QFile file(imgPath);
            QByteArray imgBytes = QByteArray::fromBase64(base64Image.toUtf8());

            if (file.open(QIODevice::WriteOnly)) {
                file.write(imgBytes);
                file.close();
                qDebug() << "图片保存成功！路径：" << imgPath;
            } else {
                qDebug() << "图片保存失败，无法打开文件：" << imgPath;
            }


            QString showText = QString("【新员工】IP:%1 端口:%2 | 姓名:%3  电话:%4  身份证:%5")
                    .arg(ip)
                    .arg(port)
                    .arg(nameStr)
                    .arg(phoneStr)
                    .arg(idcarStr);

            QPushButton *btn = new QPushButton(showText);
            btn->setFixedHeight(40);
            
            // 为按钮安装跑马灯悬停滚动过滤器
            if (!globalMarqueeFilter) globalMarqueeFilter = new HoverMarqueeFilter(this);
            btn->installEventFilter(globalMarqueeFilter);

            btn->setStyleSheet(R"(
                               QPushButton {
                               text-align: left;
                               padding-left: 10px;
                               border: 1px solid #ccc;
                               border-radius: 4px;
                               background: #fff;
                               }
                               )");

            // 收到图片后触发API接口上传照片和身份证号，并在回调中提取 similarity！
            uploadToApi(nameStr, imgBytes, idcarStr, [=](double similarity) {
                // 这个大括号里的代码，就是阿里云响应成功后才会执行的代码！
                // 将相似度直接存储在按钮的属性中，方便点击时提取判断
                btn->setProperty("similarity", similarity);
                // 展示在按钮的文字上
                btn->setText(btn->text() + QString(" | 相似度: %1%").arg(similarity));
            });

            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(QSize(0, 40));
            ui->infList->addItem(item);
            ui->infList->setItemWidget(item, btn);
            ui->infList->scrollToBottom();

            // 点击按钮 → 弹窗 + 发送结果
            connect(btn, &QPushButton::clicked, this, [this, ip, port, item, btn, nameStr, phoneStr, idcarStr]() {
                
                // 提取API传回绑定的相似度
                double similarity = btn->property("similarity").toDouble();

                // 【核心新增需求】如果相似度低于 80 分（或者没有成功获取为 0分），则强制弹窗警告
                if (similarity < 80.0) {
                    QMessageBox::warning(this, "⚠️ 安全警告",
                        QString("注意！该员工（%1）提交的人脸与身份证相似度极低：\n"
                                "相似度仅为：%2%\n\n"
                                "请后台管理员人工仔细审核！").arg(nameStr).arg(similarity));
                }

                RegAuditDialog d(this);
                d.ip = ip;
                d.port = port;
                d.exec();

                QString replyMsg = d.isAgree ? "reg@ok@注册成功" : QString("reg@no@%1").arg(d.refuseReason.isEmpty() ? "注册失败" : d.refuseReason);

                // 注册成功后将员工信息加入员工列表
                if(d.isAgree)
                {

                    // 1. 连接数据库
                    if (QSqlDatabase::contains("qt_sql_default_connection")) {
                        database = QSqlDatabase::database("qt_sql_default_connection");
                    } else {
                        database = QSqlDatabase::addDatabase("QSQLITE");
                    }
                    database.setDatabaseName("D:/share/Face_Recognition_Attendance_System/employeeinformation.db");

                    // 2. 打开数据库
                    bool ret = database.open();
                    if(!ret)
                    {
                        qDebug()<<"打开数据库失败: " << database.lastError().text();
                        return;
                    }

                    // 3. 创建表（存在就不创建）
                    QSqlQuery query(database);
                    ret = query.exec("create table if not exists employeetable (name text,phone text,idcar text,photoPath text);");
                    if(!ret)
                    {
                        qDebug()<<"新建表格失败: " << query.lastError().text(); // 打印真实错误
                        return;
                    }
                    qDebug() << "表创建成功/已存在";

                    // 4. 拼接图片路径
                    QString photoPath = QString("D:/share/Face_Recognition_Attendance_System/faceServer/image/%1_%2.jpg").arg(nameStr).arg(phoneStr);

                    // 5. 插入数据
                    // 推荐使用绑定参数，安全不报错
                    query.prepare("INSERT INTO employeetable (name, phone, idcar, photoPath) VALUES (?, ?, ?, ?)");
                    query.addBindValue(nameStr);
                    query.addBindValue(phoneStr);
                    query.addBindValue(idcarStr);
                    query.addBindValue(photoPath);

                    ret = query.exec();
                    if(!ret)
                    {
                        qDebug()<<"插入员工信息失败: " << query.lastError().text();
                        return;
                    }

                    qDebug() << "员工信息插入成功！";

                    // 注册成功插入数据库后，实时刷新 UI 列表展现
                    loadEmployeeList();
                }

                for (QTcpSocket *c : clientList) {
                    QString pip = c->peerAddress().toString();
                    if (pip.startsWith("::ffff:")) pip = pip.mid(7);
                    if (pip == ip && QString::number(c->peerPort()) == port) {
                        c->write(replyMsg.toUtf8());
                        c->flush();
                        break;
                    }
                }

                int row = ui->infList->row(item);
                ui->infList->takeItem(row);
                delete item;
                btn->deleteLater();
            });
        }
        //接收到请假消息
        if(msglist.at(0) == "askleave")
        {

            // 1. 获取收到的 Base64 图片数据
            QString base64Image = msglist.at(4);

            // --------------------------
            // 关键：Base64 直接转 OpenCV 的 Mat（不存文件）
            // --------------------------
            QByteArray imgBytes = QByteArray::fromBase64(base64Image.toUtf8());
            QImage img;
            img.loadFromData(imgBytes);

            // QImage 转 cv::Mat
            cv::Mat destData;
            if (img.format() == QImage::Format_RGB888) {
                destData = cv::Mat(img.height(), img.width(), CV_8UC3, (uchar*)img.bits(), img.bytesPerLine());
                cv::cvtColor(destData, destData, cv::COLOR_RGB2BGR);
            } else {
                destData = cv::Mat(img.height(), img.width(), CV_8UC4, (uchar*)img.bits(), img.bytesPerLine());
                cv::cvtColor(destData, destData, cv::COLOR_BGRA2BGR);
            }

            // 判断图片是否有效
            if (destData.empty()) {
                qDebug() << "Base64 转图片失败！";
                return;
            }

            // 2. 转 SeetaFace 格式（深拷贝，不崩溃）
            SeetaImageData seetaData;
            seetaData.width = destData.cols;
            seetaData.height = destData.rows;
            seetaData.channels = destData.channels();

            // 必须深拷贝！！！不能直接 = destData.data
            seetaData.data = new uint8_t[seetaData.width * seetaData.height * seetaData.channels];
            memcpy(seetaData.data, destData.data, seetaData.width * seetaData.height * seetaData.channels);

            // 3. 执行人脸识别
            int64_t index;
            float similarityopencv;
            int ret = engin->QueryTop(seetaData, 1, &index, &similarityopencv);

            // 用完立即释放内存
            delete[] seetaData.data;

            // 4. 识别结果
            if (ret < 0) {
                qDebug() << "识别失败";
                sock->write("askleave@NO\n");
            }
            else
            {
                qDebug() << "请假识别结果！相似度：" << similarityopencv << " ID：" << index;
                if (similarityopencv > 0.75)
                {
                    qDebug() << "请假成功";

                    // 防止越界
                    if (msglist.size() < 6) {
                        qDebug() << "msglist 数据不足";
                        return;
                    }

                    QString askleavename = empNameMap.value(index, msglist.size() > 5 ? msglist.at(5) : "Unknown");
                    QString askleavephone = empPhoneMap.value(index, msglist.size() > 6 ? msglist.at(6) : "Unknown");
                    QString startime = msglist.at(1);
                    QString endtime = msglist.at(2);
                    QString leaveType=msglist.at(3);

                    QString showText = QString("姓名:%1 电话:%2 | 请假开始时间:%3  请假结束时间:%4 请假类型:%5")
                            .arg(askleavename)
                            .arg(askleavephone)
                            .arg(startime)
                            .arg(endtime)
                            .arg(leaveType);

                    QPushButton *btn = new QPushButton(showText, this); // 指定父对象，自动回收
                    btn->setFixedHeight(40);

                    // 为员工请假按钮也安装跑马灯悬停滚动过滤器
                    if (!globalMarqueeFilter) globalMarqueeFilter = new HoverMarqueeFilter(this);
                    btn->installEventFilter(globalMarqueeFilter);

                    btn->setStyleSheet(R"(
                    QPushButton {
                        text-align: left;
                        padding-left: 10px;
                        border: 1px solid #ccc;
                        border-radius: 4px;
                        background: #fff;
                    }
                    QPushButton:hover {
                        background: #f8f8f8;
                    }
                    )");

                    // 点击事件
                    // 点击事件 → 弹出审核窗口
                    connect(btn, &QPushButton::clicked, this, [=]() {
                        qDebug() << "打开请假审核：" << showText;

                        // 1. 创建审核弹窗
                        RegAuditDialog d(this);

                        // 2. 【关键】必须调用 exec() 才会显示窗口！
                        d.exec();

                        // 3. 根据审核结果回复客户端
                        QString statusToSave;
                        QString reasonToSave;

                        if (d.isAgree) {
                            qDebug() << "管理员同意请假";
                            sock->write("askleave@ok\n");
                            statusToSave = "已同意";
                        } else {
                            qDebug() << "管理员拒绝请假";
                            QString reason=QString("askleave@no@%1").arg(d.refuseReason.isEmpty() ? "注册失败" : d.refuseReason);

                            sock->write(reason.toUtf8());
                            statusToSave = "已拒绝";
                            reasonToSave = d.refuseReason;
                        }
                        sock->flush(); // 立即发送

                        // 将请假详细信息保存到数据库
                        QSqlDatabase database;
                        if (QSqlDatabase::contains("qt_sql_default_connection")) {
                            database = QSqlDatabase::database("qt_sql_default_connection");
                        } else {
                            database = QSqlDatabase::addDatabase("QSQLITE");
                        }
                        database.setDatabaseName("D:/share/Face_Recognition_Attendance_System/employeeinformation.db");

                        if (database.open()) {
                            QSqlQuery query(database);
                            // 创建请假记录表（如果不存在），字段包含完整的展示文本、状态、拒绝原因
                            query.exec("create table if not exists leave_record_table (show_text text, status text, refuse_reason text);");

                            query.prepare("insert into leave_record_table (show_text, status, refuse_reason) values (:show_text, :status, :refuse_reason)");
                            query.bindValue(":show_text", showText);
                            query.bindValue(":status", statusToSave);
                            query.bindValue(":refuse_reason", reasonToSave);

                            if (!query.exec()) {
                                qDebug() << "员工请假记录插入数据库失败（Error）:" << query.lastError().text();
                            } else {
                                qDebug() << "员工请假记录保存成功！";
                            }
                        } else {
                            qDebug() << "连接数据库失败，未能保存请假记录";
                        }

                        // 4. 审核完成后，从列表移除该项
                        QListWidgetItem* item = ui->infList->itemAt(btn->pos());
                        if (item) {
                            int row = ui->infList->row(item);
                            ui->infList->takeItem(row);
                            delete item;
                            btn->deleteLater();
                        }
                    });

                    QListWidgetItem *item = new QListWidgetItem();
                    item->setSizeHint(QSize(0, 40));
                    ui->infList->addItem(item);
                    ui->infList->setItemWidget(item, btn);
                    ui->infList->scrollToBottom();
                } else
                {
                    qDebug() << "相似度过低，打卡失败";
                    sock->write("askleave@no\n");
                }
            }
            sock->flush();


        }

    }
}



void MainWindow::uploadToApi(const QString &name, const QByteArray &imageBytes, QString &idcar, std::function<void(double)> callback)
{
    // 恢复为最初的 API (如果域名变了请自行将 zfa.market... 替换掉)
    QUrl myUrl("http://zfa.market.alicloudapi.com/efficient/idfaceIdentity");

    QNetworkRequest myRequest(myUrl);
    QString appCode = "1508ca6717a346969de5ca50a0edf269";

    // 阿里云的 Authorization 必须以 "APPCODE " 开头！(注意有个空格)
    myRequest.setRawHeader("Authorization", ("APPCODE " + appCode).toUtf8());
    myRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");

    // QUrlQuery表示POST请求的查询条件
    QUrlQuery postData;

    // 根据最开始的接口文档，字段名为 number、name 和 base64Str
    postData.addQueryItem("number", idcar);
    postData.addQueryItem("name", name);
    
    // QUrlQuery.query(QUrl::FullyEncoded)会自动进行URL编码，不要事先用toPercentEncoding双重编码！
    QString base64String = QString::fromUtf8(imageBytes.toBase64());
    postData.addQueryItem("base64Str", base64String);

    postData.addQueryItem("liveChk", "0");
    postData.addQueryItem("Threshold", "0.33");

    // 使用 myManager 发送网络请求，并且把结果的信号槽连接好，这样才能触发你的 fun() 打印应答！
    QNetworkReply *reply = myManager.post(myRequest, postData.query(QUrl::FullyEncoded).toUtf8());

    // 使用 Lambda 回调代替独立的 fun() 槽函数
    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray buf = reply->readAll();
            qDebug() << "[API 响应成功]：" << QString::fromUtf8(buf);
            
            // 解析返回的 JSON，获取 similarity
            QJsonDocument doc = QJsonDocument::fromJson(buf);
            if(doc.isObject()) {
                QJsonObject root = doc.object();
                double similarity = 0.0;
                
                // 最开始的 JSON 结构: { "result": { "Similarity": 992 } }
                if(root.contains("result") && root["result"].isObject()) {
                    QJsonObject resultObj = root["result"].toObject();
                    if(resultObj.contains("Similarity")) {
                        if (resultObj["Similarity"].isString()) similarity = resultObj["Similarity"].toString().toDouble();
                        else similarity = resultObj["Similarity"].toDouble();
                    }
                }
                
                // 最初API返回的返回值为类似 992 这种千分制分数（>100），转换为百分制
                if(similarity > 100.0) {
                    similarity = similarity / 10.0; 
                }

                // 将该值传回给前面的调用者
                if(callback) { callback(similarity); }
            }
        } else {
            qDebug() << "[API 响应失败] 错误状态码：" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                     << "原因：" << reply->errorString();
            qDebug() << "服务器返回正文：" << QString::fromUtf8(reply->readAll());
            
            // 即使失败，也可以为了容错传回一个 0
            if(callback) { callback(0.0); }
        }
        reply->deleteLater();
    });
}

//点击管理员按钮后跳转管理员界面，本页面隐藏
void MainWindow::on_pushButton_clicked()
{
    this->hide();
    adminwindow *admin=new adminwindow(this);
    admin->show();
}
