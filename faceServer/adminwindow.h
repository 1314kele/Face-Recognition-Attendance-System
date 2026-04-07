#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class adminwindow;
}

class adminwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminwindow(QWidget *parent = nullptr);
    ~adminwindow();

private slots:

    void on_deletebt_clicked();

    void on_updatabt_clicked();

    void on_findbt_clicked();

    void on_addbt_clicked();
    void reflushList();

    void on_backbt_clicked();

private:
    Ui::adminwindow *ui;
    QSqlDatabase database;
};

#endif // ADMINWINDOW_H
