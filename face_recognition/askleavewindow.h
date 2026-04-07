#ifndef ASKLEAVEWINDOW_H
#define ASKLEAVEWINDOW_H

#include <QMainWindow>

namespace Ui {
class askLeavewindow;
}

class askLeavewindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit askLeavewindow(QWidget *parent = nullptr);
    ~askLeavewindow();

private:
    Ui::askLeavewindow *ui;
};

#endif // ASKLEAVEWINDOW_H
