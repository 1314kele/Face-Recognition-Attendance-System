/********************************************************************************
** Form generated from reading UI file 'askleavewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASKLEAVEWINDOW_H
#define UI_ASKLEAVEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_askLeavewindow
{
public:
    QWidget *centralwidget;
    QCalendarWidget *calendarWidget;
    QPushButton *backbt;
    QLineEdit *starTimele;
    QLineEdit *endTimele;
    QPushButton *applyLeavebt;
    QPushButton *clearbt1;
    QPushButton *clearbt2;
    QTimeEdit *timeEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *askLeavewindow)
    {
        if (askLeavewindow->objectName().isEmpty())
            askLeavewindow->setObjectName(QString::fromUtf8("askLeavewindow"));
        askLeavewindow->resize(800, 600);
        centralwidget = new QWidget(askLeavewindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        calendarWidget = new QCalendarWidget(centralwidget);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));
        calendarWidget->setGeometry(QRect(0, 0, 291, 241));
        backbt = new QPushButton(centralwidget);
        backbt->setObjectName(QString::fromUtf8("backbt"));
        backbt->setGeometry(QRect(720, 0, 75, 23));
        starTimele = new QLineEdit(centralwidget);
        starTimele->setObjectName(QString::fromUtf8("starTimele"));
        starTimele->setGeometry(QRect(410, 40, 191, 41));
        endTimele = new QLineEdit(centralwidget);
        endTimele->setObjectName(QString::fromUtf8("endTimele"));
        endTimele->setGeometry(QRect(410, 100, 191, 41));
        applyLeavebt = new QPushButton(centralwidget);
        applyLeavebt->setObjectName(QString::fromUtf8("applyLeavebt"));
        applyLeavebt->setGeometry(QRect(460, 170, 75, 23));
        clearbt1 = new QPushButton(centralwidget);
        clearbt1->setObjectName(QString::fromUtf8("clearbt1"));
        clearbt1->setGeometry(QRect(610, 50, 75, 23));
        clearbt2 = new QPushButton(centralwidget);
        clearbt2->setObjectName(QString::fromUtf8("clearbt2"));
        clearbt2->setGeometry(QRect(610, 110, 75, 23));
        timeEdit = new QTimeEdit(centralwidget);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setGeometry(QRect(0, 240, 118, 22));
        askLeavewindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(askLeavewindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        askLeavewindow->setMenuBar(menubar);
        statusbar = new QStatusBar(askLeavewindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        askLeavewindow->setStatusBar(statusbar);

        retranslateUi(askLeavewindow);

        QMetaObject::connectSlotsByName(askLeavewindow);
    } // setupUi

    void retranslateUi(QMainWindow *askLeavewindow)
    {
        askLeavewindow->setWindowTitle(QCoreApplication::translate("askLeavewindow", "MainWindow", nullptr));
        backbt->setText(QCoreApplication::translate("askLeavewindow", "\350\277\224\345\233\236", nullptr));
        starTimele->setPlaceholderText(QCoreApplication::translate("askLeavewindow", "\350\257\267\345\201\207\350\265\267\346\255\242\346\227\266\351\227\264", nullptr));
        endTimele->setPlaceholderText(QCoreApplication::translate("askLeavewindow", "\346\210\252\346\255\242\346\227\266\351\227\264", nullptr));
        applyLeavebt->setText(QCoreApplication::translate("askLeavewindow", "\347\224\263\350\257\267\350\257\267\345\201\207", nullptr));
        clearbt1->setText(QCoreApplication::translate("askLeavewindow", "\346\270\205\351\231\244", nullptr));
        clearbt2->setText(QCoreApplication::translate("askLeavewindow", "\346\270\205\351\231\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class askLeavewindow: public Ui_askLeavewindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKLEAVEWINDOW_H
