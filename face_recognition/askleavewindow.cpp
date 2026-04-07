#include "askleavewindow.h"
#include "ui_askleavewindow.h"
#include <QDate>
#include <QDateTime>
#include <QDebug>

askLeavewindow::askLeavewindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::askLeavewindow)
{
    ui->setupUi(this);

        // 1. 在窗口构造函数中绑定信号槽
        ui->calendarWidget->setSelectedDate(QDate::currentDate()); // 默认选中今天
        connect(ui->calendarWidget, &QCalendarWidget::selectionChanged, this, [=]() {

            // 点击日期时触发，获取选中的日期
            QDate selectedDate = ui->calendarWidget->selectedDate();
            QTime selectedTime = ui->timeEdit->time();
            QString timeStr = selectedTime.toString();
            // 格式化输出：2026-04-07 或 2026年04月07日
            QString dateStr = selectedDate.toString("yyyy-MM-dd");

            // 1. 获取你日历选中的日期（你已经拿到的 dateStr）
            QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");

            // 2. 获取时间组件里的时间
            QTime time = ui->timeEdit->time(); // 就是你截图那个 QTimeEdit

            // 3. 合并成 完整日期时间（核心！）
            QDateTime dateTime = QDateTime(date, time);

            // 4. 转成字符串（数据库、显示都能用）
            QString dateTimeStr = dateTime.toString("yyyy-MM-dd HH:mm:ss");

            qDebug() << "选中的日期：" << dateTimeStr;

            if(ui->starTimele->text().isEmpty())
            {
                 ui->starTimele->setText(dateTimeStr);
            }
            else
            {
                ui->endTimele->setText(dateTimeStr);
            }
        });
        // ========== 在这里添加：下拉框5个选项 ==========
           ui->comboBox_leaveType->clear();
           QStringList types = {
               "事假",
               "病假",
               "年假",
               "婚假",
               "其他假"
           };
           ui->comboBox_leaveType->addItems(types);
}

askLeavewindow::~askLeavewindow()
{
    delete ui;
}

void askLeavewindow::on_backbt_clicked()
{
    this->close();
    parentWidget()->show();
}
//清空起止时间输入框
void askLeavewindow::on_clearbt1_clicked()
{
    ui->starTimele->clear();
}
//清空截至时间输入框
void askLeavewindow::on_clearbt2_clicked()
{
     ui->endTimele->clear();
}

//申请请假按钮
void askLeavewindow::on_applyLeavebt_clicked()
{
    //点击按钮后获取输入框的文本
    QString starStr=ui->starTimele->text();
    QString endStr=ui->endTimele->text(); // 刚刚这里你写成了 starTimele，我帮你订正了
    // 拿到下拉框选中的类型
        QString leaveType = ui->comboBox_leaveType->currentText();

     // 这一行就是
    QString msg = QString("askleave@%1@%2@%3").arg(starStr).arg(endStr).arg(leaveType);
    qDebug()<<msg;
    // 3. 发射信号，把数据丢出去！
    emit sendLeaveData(msg);

    // 提交后可以关闭请假窗口并显示主窗口
    this->close();
    parentWidget()->show();
}
