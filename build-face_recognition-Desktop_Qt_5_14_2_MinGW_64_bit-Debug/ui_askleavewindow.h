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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
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
    QTimeEdit *timeEdit;
    QWidget *widget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *starTimele;
    QPushButton *clearbt1;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *endTimele;
    QPushButton *clearbt2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *applyLeavebt;
    QComboBox *comboBox_leaveType;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *askLeavewindow)
    {
        if (askLeavewindow->objectName().isEmpty())
            askLeavewindow->setObjectName(QString::fromUtf8("askLeavewindow"));
        askLeavewindow->resize(800, 480);
        centralwidget = new QWidget(askLeavewindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        calendarWidget = new QCalendarWidget(centralwidget);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));
        calendarWidget->setGeometry(QRect(0, 0, 291, 241));
        backbt = new QPushButton(centralwidget);
        backbt->setObjectName(QString::fromUtf8("backbt"));
        backbt->setGeometry(QRect(720, 0, 75, 23));
        timeEdit = new QTimeEdit(centralwidget);
        timeEdit->setObjectName(QString::fromUtf8("timeEdit"));
        timeEdit->setGeometry(QRect(0, 240, 118, 22));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(370, 50, 218, 89));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        starTimele = new QLineEdit(widget);
        starTimele->setObjectName(QString::fromUtf8("starTimele"));

        horizontalLayout->addWidget(starTimele);

        clearbt1 = new QPushButton(widget);
        clearbt1->setObjectName(QString::fromUtf8("clearbt1"));

        horizontalLayout->addWidget(clearbt1);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        endTimele = new QLineEdit(widget);
        endTimele->setObjectName(QString::fromUtf8("endTimele"));

        horizontalLayout_2->addWidget(endTimele);

        clearbt2 = new QPushButton(widget);
        clearbt2->setObjectName(QString::fromUtf8("clearbt2"));

        horizontalLayout_2->addWidget(clearbt2);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        applyLeavebt = new QPushButton(widget);
        applyLeavebt->setObjectName(QString::fromUtf8("applyLeavebt"));

        horizontalLayout_3->addWidget(applyLeavebt);

        comboBox_leaveType = new QComboBox(widget);
        comboBox_leaveType->setObjectName(QString::fromUtf8("comboBox_leaveType"));
        comboBox_leaveType->setEditable(false);
        comboBox_leaveType->setDuplicatesEnabled(false);

        horizontalLayout_3->addWidget(comboBox_leaveType);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 1);

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
        clearbt1->setText(QCoreApplication::translate("askLeavewindow", "\346\270\205\351\231\244", nullptr));
        endTimele->setPlaceholderText(QCoreApplication::translate("askLeavewindow", "\346\210\252\346\255\242\346\227\266\351\227\264", nullptr));
        clearbt2->setText(QCoreApplication::translate("askLeavewindow", "\346\270\205\351\231\244", nullptr));
        applyLeavebt->setText(QCoreApplication::translate("askLeavewindow", "\347\224\263\350\257\267\350\257\267\345\201\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class askLeavewindow: public Ui_askLeavewindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKLEAVEWINDOW_H
