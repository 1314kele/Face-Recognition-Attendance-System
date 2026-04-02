#include "regpage.h"
#include "ui_regpage.h"

regpage::regpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::regpage)
{
    ui->setupUi(this);
}

regpage::~regpage()
{
    delete ui;
}
