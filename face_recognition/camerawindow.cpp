#include "camerawindow.h"
#include "ui_camerawindow.h"
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QVBoxLayout>
#include <QDateTime>
#include <QDir>

CameraWindow::CameraWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CameraWindow)
{
    ui->setupUi(this);

    // 必须初始化空指针！！
    camera = nullptr;

    // 加载摄像头列表
    QList<QCameraInfo> camlist = QCameraInfo::availableCameras();
    for (int i=0; i<camlist.size(); i++) {
        ui->cameraselect->addItem(camlist.at(i).deviceName());
    }
}

CameraWindow::~CameraWindow()
{
    // 彻底释放
    if (camera) {
        camera->stop();
        camera->unload();   // 卸载设备
        delete camera;
    }
    delete ui;
}

// 开启摄像头
void CameraWindow::on_oncambt_clicked()
{
    // 1. 彻底释放上一个摄像头（关键！）
    if (camera) {
        camera->stop();
        camera->unload();
        delete camera;
        camera = nullptr;
    }

    // 2. 清空显示区域
    if (ui->QVwidget->layout()) {
        QLayoutItem *item;
        while ((item = ui->QVwidget->layout()->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
    }

    // 3. 创建新摄像头
    QString camname = ui->cameraselect->currentText();
    camera = new QCamera(camname.toUtf8(), this);

    // 4. 创建显示窗口
    if (!ui->QVwidget->layout()) {
        ui->QVwidget->setLayout(new QVBoxLayout);
        ui->QVwidget->layout()->setContentsMargins(0,0,0,0);
    }

    QCameraViewfinder *viewfinder = new QCameraViewfinder(ui->QVwidget);
    ui->QVwidget->layout()->addWidget(viewfinder);

    // 5. 启动
    camera->setViewfinder(viewfinder);
    camera->start();
}

// 关闭摄像头
void CameraWindow::on_offcambt_clicked()
{
    if (camera) {
        camera->stop();
        camera->unload();  // 释放硬件资源
        delete camera;
        camera = nullptr;
    }
}

// 拍照
void CameraWindow::on_photobt_clicked()
{
    if (!camera) return;

    // 清空旧照片，并指定目标文件夹为 faceimage
    QString imgDir = "D:/share/Face_Recognition_Attendance_System/face_recognition/faceimage/";
    QDir dir(imgDir);
    if (!dir.exists()) {
        dir.mkpath(imgDir); // 如果文件夹不存在则创建
    }
    
    QStringList filters = {"*.jpg","*.jpeg","*.png"};
    for(auto f : dir.entryList(filters, QDir::Files)) dir.remove(f);

    // 准备捕获
    QCameraImageCapture *imageCapture = new QCameraImageCapture(camera, this);
    camera->setCaptureMode(QCamera::CaptureStillImage);


    // 绑定：捕获完成后再处理
    connect(imageCapture, &QCameraImageCapture::imageCaptured,
            this, &CameraWindow::onImageCaptured);

    // 开始拍照，Qt会自动将文件保存到这个路径
    imageCapture->capture(imgDir + "1.jpg");
}

// 返回主界面
void CameraWindow::on_pushButton_4_clicked()
{
    // 返回前必须释放摄像头
    if (camera) {
        camera->stop();
        camera->unload();
        delete camera;
        camera = nullptr;
    }

    this->close();
    parentWidget()->show();
}

void CameraWindow::onImageCaptured(int id, const QImage &image)
{
    QString imgDir = "D:/share/Face_Recognition_Attendance_System/face_recognition/faceimage/";
    QString savePath = imgDir + "1.jpg";

    // // 防止Qt底层的capture函数由于没有保存权限等原因失败，在此处保险起见用内存里的image再存一次
    // 直接覆盖，并且不留下多余的 temp.jpg
    image.save(savePath, "JPG", 90);

    // 现在再发路径 + 跳转
    emit sendPhotoPath(savePath);
    // 返回前必须释放摄像头
    if (camera) {
        camera->stop();
        camera->unload();
        delete camera;
        camera = nullptr;
    }
    this->close();
    parentWidget()->show();
}
