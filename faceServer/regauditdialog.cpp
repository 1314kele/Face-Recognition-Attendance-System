#include "regauditdialog.h"
#include "ui_regauditdialog.h"

// 构造函数
RegAuditDialog::RegAuditDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::regauditdialog) // 现在可以正确分配了
{
    ui->setupUi(this); // 加载按钮
    isAgree = false;    // 默认不同意
}

// 析构函数（必须实现，否则报链接错误）
RegAuditDialog::~RegAuditDialog()
{
    delete ui;
}

// 同意按钮 (对应ui中的 aggreBt)
void RegAuditDialog::on_aggreBt_clicked()
{
    isAgree = true;
    close();
}

// 不同意按钮 (对应ui中的 pushButton_2)
void RegAuditDialog::on_pushButton_2_clicked()
{
    isAgree = false;
    refuseReason = ui->textEdit->toPlainText(); // 取出文本框里输入的不同意原因
    close();
}
