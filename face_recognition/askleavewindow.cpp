#include "askleavewindow.h"
#include "ui_askleavewindow.h"

askLeavewindow::askLeavewindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::askLeavewindow)
{
    ui->setupUi(this);
}

askLeavewindow::~askLeavewindow()
{
    delete ui;
}
