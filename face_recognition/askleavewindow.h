#ifndef ASKLEAVEWINDOW_H
#define ASKLEAVEWINDOW_H

#include <QMainWindow>
#include <QDate>

namespace Ui {
class askLeavewindow;
}

class askLeavewindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit askLeavewindow(QWidget *parent = nullptr);
    ~askLeavewindow();

private slots:
    void on_backbt_clicked();//

    void on_clearbt1_clicked();

    void on_clearbt2_clicked();

    void on_applyLeavebt_clicked();

private:
    Ui::askLeavewindow *ui;

signals:
    // 增加这个自定义信号，用来把拼好的字符串传出去
    void sendLeaveData(QString leaveMsg);
};

#endif // ASKLEAVEWINDOW_H
