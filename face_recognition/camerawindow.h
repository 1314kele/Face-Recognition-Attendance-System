#ifndef CAMERAWINDOW_H
#define CAMERAWINDOW_H

#include <QMainWindow>
#include <QCameraInfo>
#include <QCamera>
#include <QVideoWidget>

namespace Ui {
class CameraWindow;
}

class CameraWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CameraWindow(QWidget *parent = nullptr);
    ~CameraWindow();

private slots:
    void on_oncambt_clicked();

    void on_offcambt_clicked();

    void on_photobt_clicked();

    void on_pushButton_4_clicked();
    void onImageCaptured(int id, const QImage &image);

private:
    Ui::CameraWindow *ui;
    QCamera *camera;
signals:
    void sendPhotoPath(QString path);

};

#endif // CAMERAWINDOW_H
