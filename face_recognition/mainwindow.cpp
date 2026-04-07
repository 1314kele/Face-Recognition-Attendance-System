#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QCoreApplication>
#include <QBuffer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //tcp通信
    //初始化套接字
    tcpsock=new QTcpSocket(this);
    //绑定
    tcpsock->bind(QHostAddress("192.168.11.81"),10000);
    //链接服务器
    tcpsock->connectToHost(QHostAddress("192.168.11.81"),20000);

    //关联readyRead()信号
    connect(tcpsock, &QTcpSocket::readyRead, this, &MainWindow::rvrecvSeerMsg);

    // QLabel 自适应图片，等比例缩放
    ui->labelImage->setScaledContents(false);
    ui->labelImage->setAlignment(Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    // 关闭服务器，释放端口
    tcpsock->close();
    delete ui;
}

void MainWindow::on_selectimagebt_clicked()
{
    QMessageBox msg(this);
    msg.setWindowTitle("提示");
    msg.setText("选择执行的操作");

    QAbstractButton *btn1 = msg.addButton("本地文件", QMessageBox::YesRole);
    QAbstractButton *btn2 = msg.addButton("拍照", QMessageBox::NoRole);

    msg.exec();

    if (msg.clickedButton() == btn1)
    {
        QString filePath = QFileDialog::getOpenFileName(
                    this,
                    "选择本地文件",
                    "C:/",
                    "所有文件 (*.*);;图片 (*.png *.jpg *.bmp);;文本文件 (*.txt)"
                    );

        if (!filePath.isEmpty()) {
            qDebug() << "选中的文件路径：" << filePath;
            QMessageBox::information(this, "成功", "已选择文件：\n" + filePath);

            // QLabel 显示图片
            QPixmap pix(filePath);
            ui->labelImage->setPixmap(pix.scaled(ui->labelImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            QMessageBox::information(this, "失败", "没有选择文件");
        }
    }
    else
    {
        this->hide();
        CameraWindow *w = new CameraWindow(this);
        connect(w, &CameraWindow::sendPhotoPath, this, &MainWindow::showPhoto);
        w->show();
    }
}

// 拍照返回显示
void MainWindow::showPhoto(QString path)
{
    this->show();

    // 显示照片
    QPixmap pix(path);
    ui->labelImage->setPixmap(pix.scaled(ui->labelImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

//注册发送消息给服务器
void MainWindow::on_regbt_clicked()
{
    //判断消息框是否为空
    if(ui->regname->text().isEmpty() || ui->regphone->text().isEmpty() || ui->regidcar->text().isEmpty() )
    {
        QMessageBox::warning(this, "提示", "请输入完整的用户信息！");
        return;
    }
    QString name=ui->regname->text();
    QString phone=ui->regphone->text(); // 这是一个手机号，修改了原代码里易混淆的photo命名
    QString idcar=ui->regidcar->text();
    //判断手机号和身份证号是否符合长度要求
    if(name.size()!=11 && idcar.size()!=18)
    {
        QMessageBox::warning(this, "提示", "手机号或者身份证不合规！");
        return;
    }
    if (ui->labelImage->pixmap() == nullptr || ui->labelImage->pixmap()->isNull())
    {
        // 空的 → 没有图片
        QMessageBox::warning(this, "提示", "请先上传员工照片！");
        return;
    }
    else
    {
        // 2. 将图片转换为 Base64 编码的字符串（文本形式的图片数据）
        QByteArray imageBytes;
        QBuffer buffer(&imageBytes);
        buffer.open(QIODevice::WriteOnly);
        // 这里以 JPG 格式提取并压缩写入 buffer（90是压缩质量
        ui->labelImage->pixmap()->toImage().save(&buffer, "JPG", 70);
        QString base64Image = QString(imageBytes.toBase64()); // 转成 Base64 字符

        // 3. 将图片数据一并打包到发送的字符串中。格式变为 reg@姓名@手机号@身份证@照片Base64数据
        // 注意：这样拼接之后，字符串会变得非常大
        QString msg = QString("reg@%1@%2@%3@%4\n").arg(name).arg(phone).arg(idcar).arg(base64Image);
        tcpsock->write(msg.toUtf8());
    }
}

void MainWindow::rvrecvSeerMsg()
{
    QByteArray buf=tcpsock->readAll();
    //把字节数组转成字符串
    QString str = QString(buf).trimmed(); // 去除末尾的换行符
    //切割字符串判断收到的是什么意思
    auto msglist=str.split("@");
    //注册返回信息
    if(msglist.at(0)=="reg")//回复注册信息
    {
        if(msglist.at(1)=="ok")//注册成功
        {
            QMessageBox::information(this, "注册成功", "恭喜你注册员工信息成功");
        }
        else
        {
            QMessageBox::warning(this, "注册失败", msglist.at(2));
        }
    }
    //打卡返回信息
    if(msglist.at(0)=="clockin")
    {
        if(msglist.at(1)=="ok")//打卡成功
        {
            QString statusMsg = "打卡成功";
            if (msglist.size() > 2) {
                statusMsg = msglist.at(2); // 取出考勤状态（迟到/早退/下班等）
            }
            QMessageBox::information(this, "打卡结果", statusMsg);
        }
        else if(msglist.at(1)=="NO")
        {
            QMessageBox::warning(this, "打卡结果","照片识别失败");
        }
        else
        {
            QMessageBox::warning(this, "打卡结果","打卡失败");
        }
    }

    if(msglist.at(0)=="askleave")
    {
        if(msglist.at(1)=="ok")//请假成功
        {
            QMessageBox::information(this, "请假结果", "请假成功");
        }
        else if(msglist.at(1)=="NO")
        {
            QMessageBox::warning(this, "请假结果","请上传人脸照片");
        }
        else
        {
            QMessageBox::warning(this, "请假结果",msglist.at(2));
        }
    }

}


//打卡按钮响应函数
void MainWindow::on_pushButton_clicked()
{
    //判断是否上传了照片
    if (ui->labelImage->pixmap() == nullptr || ui->labelImage->pixmap()->isNull())
    {
        // 空的 → 没有图片
        QMessageBox::warning(this, "提示", "请先上传员工照片！");
        return;
    }
    else
    {
        // 1. 将图片转换为 Base64 编码的字符串（文本形式的图片数据）
        QByteArray imageBytes;
        QBuffer buffer(&imageBytes);
        buffer.open(QIODevice::WriteOnly);
        // 这里以 JPG 格式提取并压缩写入 buffer
        ui->labelImage->pixmap()->toImage().save(&buffer, "JPG", 70);
        QString base64Image = QString(imageBytes.toBase64()); // 转成 Base64 字符

        // 2. 将图片数据一并打包到发送的字符串中。
        QString msg = QString("clockin@%1\n").arg(base64Image);
        tcpsock->write(msg.toUtf8());
    }
}


//请假按钮槽函数
void MainWindow::on_askLeavebt_clicked()
{

    askLeavewindow *askLeavepage=new askLeavewindow(this);

    // 【新增下面这段】拦截刚才写的那个 sendLeaveData 信号
    connect(askLeavepage, &askLeavewindow::sendLeaveData, this, [=](QString msg) {

        //判断是否上传了照片
        if (ui->labelImage->pixmap() == nullptr || ui->labelImage->pixmap()->isNull() || ui->regname->text().isEmpty() || ui->regphone->text().isEmpty())
        {
            // 空的 → 没有图片
            QMessageBox::warning(this, "提示", "请先输入员工姓名、手机号、上传员工照片！");
            return;
        }
        else
        {

            this->hide();
            // 2. 将图片转换为 Base64 编码的字符串（文本形式的图片数据）
            QByteArray imageBytes;
            QBuffer buffer(&imageBytes);
            buffer.open(QIODevice::WriteOnly);
            // 这里以 JPG 格式提取并压缩写入 buffer（90是压缩质量
            ui->labelImage->pixmap()->toImage().save(&buffer, "JPG", 70);
            QString base64Image = QString(imageBytes.toBase64()); // 转成 Base64 字符
            QString MSG = QString("%1@%2@%3@%4\n").arg(msg).arg(base64Image).arg(ui->regname->text()).arg(ui->regphone->text());

            // 当收到请假页面的数据时，主窗口直接通过 tcp 发给服务器！
            if(tcpsock->state() == QAbstractSocket::ConnectedState) {
                tcpsock->write(MSG.toUtf8());
                tcpsock->flush();
            }
            //qDebug()<<MSG;
        }

    });

    askLeavepage->show();

}


