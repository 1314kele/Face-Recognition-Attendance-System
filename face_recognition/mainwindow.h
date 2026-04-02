#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camerawindow.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QPixmap>
#include <QSerialPort>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_selectimagebt_clicked();
    void showPhoto(QString path);
    void on_regbt_clicked();
    void rvrecvSeerMsg();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpsock;
    QString imagePath;
};

#endif // MAINWINDOW_H
