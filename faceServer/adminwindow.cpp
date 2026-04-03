#include "adminwindow.h"
#include "ui_adminwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

adminwindow::adminwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminwindow)
{
    ui->setupUi(this);

    // 构造函数里调用刷新
    reflushList();
}

adminwindow::~adminwindow()
{
    delete ui;
}

// 刷新列表（修复版）
void adminwindow::reflushList()
{
    // 清空
    ui->listWidget->clear();

    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        database = QSqlDatabase::addDatabase("QSQLITE");
    }

    database.setDatabaseName("D:/share/Face_Recognition_Attendance_System/employeeinformation.db");

    if (!database.open()) {
        qDebug() << "刷新失败：打不开数据库";
        return;
    }

    QSqlQuery query(database);
    QString sql = "SELECT name, phone, idcar, photoPath FROM employeetable";

    if (query.exec(sql)) {
        while (query.next()) {
            QString nameStr = query.value(0).toString();
            QString phoneStr = query.value(1).toString();
            QString idcarStr = query.value(2).toString();

            // 拼接显示（只显示3个字段）
            QString info = QString("姓名:%1  电话:%2  身份证:%3")
                               .arg(nameStr)
                               .arg(phoneStr)
                               .arg(idcarStr);

            ui->listWidget->addItem(info);
        }
        qDebug() << "✅ 员工列表刷新成功";
    } else {
        qDebug() << "查询失败：" << query.lastError().text();
    }
}

// ====================== 添加员工 ======================
void adminwindow::on_addbt_clicked()
{
    QString name = ui->namele->text().trimmed();
    QString phone = ui->phonele->text().trimmed();
    QString idcar = ui->idcarle->text().trimmed();
    QString path = ui->photoPathle->text().trimmed();

    // 判空
    if(name.isEmpty() || phone.isEmpty() || idcar.isEmpty() || path.isEmpty()){
        QMessageBox::warning(this,"提示","信息不能为空！");
        return;
    }

    QSqlQuery query(database);
    query.prepare("INSERT INTO employeetable (name, phone, idcar, photoPath) VALUES (?,?,?,?)");
    query.addBindValue(name);
    query.addBindValue(phone);
    query.addBindValue(idcar);
    query.addBindValue(path);

    if(!query.exec()){
        qDebug() << "添加失败：" << query.lastError().text();
        return;
    }

    QMessageBox::information(this,"成功","添加成功！");
    reflushList(); // 刷新列表 ✅
}

// ====================== 删除员工 ======================
void adminwindow::on_deletebt_clicked()
{
    QString phone = ui->phonele->text().trimmed();
    if(phone.isEmpty()){
        QMessageBox::warning(this,"提示","请输入要删除的手机号！");
        return;
    }

    QSqlQuery query(database);
    query.prepare("DELETE FROM employeetable WHERE phone=?");
    query.addBindValue(phone);

    if(!query.exec()){
        qDebug() << "删除失败：" << query.lastError().text();
        return;
    }

    QMessageBox::information(this,"成功","删除成功！");
    reflushList(); // 刷新列表 ✅
}

// ====================== 修改员工 ======================
void adminwindow::on_updatabt_clicked()
{
    QString ophone = ui->oldphone->text().trimmed();
    QString nname = ui->namele->text().trimmed();
    QString nphone = ui->phonele->text().trimmed();
    QString nidcar = ui->idcarle->text().trimmed();
    QString npath = ui->photoPathle->text().trimmed();

    if(ophone.isEmpty()){
        QMessageBox::warning(this,"提示","请输入旧手机号！");
        return;
    }

    QSqlQuery query;
    if(!nname.isEmpty()){
        query.prepare("UPDATE employeetable SET name=? WHERE phone=?");
        query.addBindValue(nname);
        query.addBindValue(ophone);
        query.exec();
    }
    if(!nphone.isEmpty()){
        query.prepare("UPDATE employeetable SET phone=? WHERE phone=?");
        query.addBindValue(nphone);
        query.addBindValue(ophone);
        query.exec();
        ophone = nphone;
    }
    if(!nidcar.isEmpty()){
        query.prepare("UPDATE employeetable SET idcar=? WHERE phone=?");
        query.addBindValue(nidcar);
        query.addBindValue(ophone);
        query.exec();
    }
    if(!npath.isEmpty()){
        query.prepare("UPDATE employeetable SET photoPath=? WHERE phone=?");
        query.addBindValue(npath);
        query.addBindValue(ophone);
        query.exec();
    }

    QMessageBox::information(this,"成功","修改成功！");
    reflushList(); // 刷新列表 ✅
}

// ====================== 查询员工 ======================
void adminwindow::on_findbt_clicked()
{
    ui->findInfmation->clear();
    ui->findInfmation->setRowCount(0);

    QStringList header = {"姓名","手机号","身份证号","图片路径"};
    ui->findInfmation->setHorizontalHeaderLabels(header);

    QSqlQuery query(database);
    query.exec("SELECT name,phone,idcar,photoPath FROM employeetable");

    int i=0;
    while(query.next()){
        ui->findInfmation->setItem(i,0,new QTableWidgetItem(query.value(0).toString()));
        ui->findInfmation->setItem(i,1,new QTableWidgetItem(query.value(1).toString()));
        ui->findInfmation->setItem(i,2,new QTableWidgetItem(query.value(2).toString()));
        ui->findInfmation->setItem(i,3,new QTableWidgetItem(query.value(3).toString()));
        i++;
    }
}
