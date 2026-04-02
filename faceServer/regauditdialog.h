#ifndef REGAUDITDIALOG_H
#define REGAUDITDIALOG_H

#include <QDialog>

// 使用前置声明，避免在此处直接 include UI 头文件产生循环依赖和命名冲突
namespace Ui {
class regauditdialog;
}

class RegAuditDialog : public QDialog
{
    Q_OBJECT


public:
    explicit RegAuditDialog(QWidget *parent = nullptr);
    ~RegAuditDialog(); // 声明析构函数

    QString ip;
    QString port;
    bool isAgree;
    QString refuseReason;

private slots:
    void on_aggreBt_clicked();
    void on_pushButton_2_clicked();

private:
    // 这里必须声明 Ui::regauditdialog 指针
    Ui::regauditdialog *ui;
};

#endif // REGAUDITDIALOG_H
