#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMap>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <functional>

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
    void newClientLink();        // 新客户端连接
    void recvClientMsg();        // 接收客户端消息
    // void fun(QNetworkReply *rep); // 删掉独立的 fun()，改用 Lambda 回调直接处理，方便传参
    
private:
    // 增加一个 callback 参数，用于接口收到响应时把相似度抛回给前面的代码
    void uploadToApi(const QString &name, const QByteArray &imageBytes, QString &idcar, std::function<void(double)> callback);

private:
    Ui::MainWindow *ui;

    QTcpServer *tcpserver;       // 服务器对象
    QTcpSocket *tcpsock;         // 客户端套接字
    QList<QTcpSocket*> clientList; // 保存所有客户端
    
    // 用于解决TCP粘包/半包的缓冲区
    QMap<QTcpSocket*, QByteArray> bufferMap;
    // 网络请求管理器，用于向API发送HTTP/POST请求
    QNetworkAccessManager *networkManager;
    QNetworkAccessManager myManager;
};

#endif // MAINWINDOW_H
