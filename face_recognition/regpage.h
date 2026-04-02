#ifndef REGPAGE_H
#define REGPAGE_H

#include <QWidget>

namespace Ui {
class regpage;
}

class regpage : public QWidget
{
    Q_OBJECT

public:
    explicit regpage(QWidget *parent = nullptr);
    ~regpage();

private:
    Ui::regpage *ui;
};

#endif // REGPAGE_H
