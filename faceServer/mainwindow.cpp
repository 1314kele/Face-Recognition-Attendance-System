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
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>

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

                    // 释放内存
                    delete[] data.data;
                    qDebug()<<id;

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
                    qDebug() << "打卡成功";
                    sock->write("clockin@ok\n");
                } else {
                    qDebug() << "相似度过低，打卡失败";
                    sock->write("clock@no\n");
                }
            }
            sock->flush();
        }

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


            // 收到图片后触发API接口上传照片和身份证号，并在回调中提取 similarity！
            uploadToApi(nameStr, imgBytes, idcarStr, [=](double similarity) {
                // 这个大括号里的代码，就是阿里云响应成功后才会执行的代码！
                // 将相似度直接存储在按钮的属性中，方便点击时提取判断
//                btn->setProperty("similarity", similarity);
                // 展示在按钮的文字上
//                btn->setText(btn->text() + QString(" | 相似度: %1%").arg(similarity));
            });

            QString showText = QString("【新员工】IP:%1 端口:%2 | 姓名:%3  电话:%4  身份证:%5")
                    .arg(ip)
                    .arg(port)
                    .arg(nameStr)
                    .arg(phoneStr)
                    .arg(idcarStr);

            QPushButton *btn = new QPushButton(showText);
            btn->setFixedHeight(40);
            btn->setStyleSheet(R"(
                QPushButton {
                    text-align: left;
                    padding-left: 10px;
                    border: 1px solid #ccc;
                    border-radius: 4px;
                    background: #fff;
                }
            )");

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
                // 【免计费修改】暂时注释掉该验证，由于API被跳过，直接默认信任，不弹窗中断流程
                /*
                if (similarity < 80.0) {
                    QMessageBox::warning(this, "⚠️ 安全警告", 
                        QString("注意！该员工（%1）提交的人脸与身份证相似度极低：\n"
                                "相似度仅为：%2%\n\n"
                                "请后台管理员人工仔细审核！").arg(nameStr).arg(similarity));
                }
                */

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
    }
}

void MainWindow::uploadToApi(const QString &name, const QByteArray &imageBytes, QString &idcar, std::function<void(double)> callback)
{
    // 【免计费修改】暂时屏蔽真实的 API 调用，直接返回一个高分相似度，跳过验证
    if (callback) {
        callback(99.9); // 模拟返回 99.9% 相似度
    }
    return;

    //供应商：杭州快证签科技有限公司 (如果是这家的话，请核对API路径和参数)
    QUrl myUrl("http://zfa.market.alicloudapi.com/efficient/idfaceIdentity");

    QNetworkRequest myRequest(myUrl);
    QString appCode = "1508ca6717a346969de5ca50a0edf269";

    // 阿里云的 Authorization 必须以 "APPCODE " 开头！(注意有个空格)
    myRequest.setRawHeader("Authorization", ("APPCODE " + appCode).toUtf8());
    myRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");

    // QUrlQuery表示POST请求的查询条件
    QUrlQuery postData;

    // 极其重要的修复：根据你刚发的阿里云接口文档，参数名必须严格照着文档写！
    postData.addQueryItem("idcar", idcar); // 文档中叫 number，不是 idcard---------------------记得改回来
    postData.addQueryItem("name", name);    // 文档中叫 name
    
    // 文档里说直接传base64，并且字段名叫做 base64Str
    QString base64String = QString::fromUtf8(imageBytes.toBase64());
    postData.addQueryItem("base64Str", QUrl::toPercentEncoding(base64String));

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
                
                // 根据具体厂商返回的 JSON 结构，这里假设相似度在根节点或者 data 节点里。
                // 如果在 { "data": { "similarity": 88.5 } }
                if(root.contains("data") && root["data"].isObject()) {
                    QJsonObject data = root["data"].toObject();
                    if(data.contains("similarity")) {
                        similarity = data["similarity"].toDouble();
                    }
                } else if(root.contains("similarity")) {
                    similarity = root["similarity"].toDouble();
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
