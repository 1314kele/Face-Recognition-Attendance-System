/********************************************************************************
** Form generated from reading UI file 'camerawindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERAWINDOW_H
#define UI_CAMERAWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CameraWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QComboBox *cameraselect;
    QPushButton *pushButton_4;
    QWidget *QVwidget;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QPushButton *oncambt;
    QPushButton *offcambt;
    QPushButton *photobt;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *CameraWindow)
    {
        if (CameraWindow->objectName().isEmpty())
            CameraWindow->setObjectName(QString::fromUtf8("CameraWindow"));
        CameraWindow->resize(800, 480);
        centralwidget = new QWidget(CameraWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 224, 27));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font.setPointSize(20);
        label->setFont(font);
        cameraselect = new QComboBox(centralwidget);
        cameraselect->setObjectName(QString::fromUtf8("cameraselect"));
        cameraselect->setGeometry(QRect(10, 43, 421, 33));
        cameraselect->setFont(font);
        pushButton_4 = new QPushButton(centralwidget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(700, 10, 75, 23));
        QVwidget = new QWidget(centralwidget);
        QVwidget->setObjectName(QString::fromUtf8("QVwidget"));
        QVwidget->setGeometry(QRect(11, 83, 421, 361));
        QVwidget->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(460, 238, 251, 171));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        oncambt = new QPushButton(widget);
        oncambt->setObjectName(QString::fromUtf8("oncambt"));
        oncambt->setFont(font);

        verticalLayout->addWidget(oncambt);

        offcambt = new QPushButton(widget);
        offcambt->setObjectName(QString::fromUtf8("offcambt"));
        offcambt->setFont(font);

        verticalLayout->addWidget(offcambt);

        photobt = new QPushButton(widget);
        photobt->setObjectName(QString::fromUtf8("photobt"));
        photobt->setFont(font);

        verticalLayout->addWidget(photobt);

        CameraWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(CameraWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        CameraWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(CameraWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        CameraWindow->setStatusBar(statusbar);

        retranslateUi(CameraWindow);

        QMetaObject::connectSlotsByName(CameraWindow);
    } // setupUi

    void retranslateUi(QMainWindow *CameraWindow)
    {
        CameraWindow->setWindowTitle(QCoreApplication::translate("CameraWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("CameraWindow", "\351\200\211\346\213\251\344\275\277\347\224\250\347\232\204\346\221\204\345\203\217\345\244\264", nullptr));
        pushButton_4->setText(QCoreApplication::translate("CameraWindow", "\350\277\224\345\233\236", nullptr));
        oncambt->setText(QCoreApplication::translate("CameraWindow", "\345\220\257\345\212\250\346\221\204\345\203\217\345\244\264", nullptr));
        offcambt->setText(QCoreApplication::translate("CameraWindow", "\345\205\263\351\227\255\346\221\204\345\203\217\345\244\264", nullptr));
        photobt->setText(QCoreApplication::translate("CameraWindow", "\346\213\215\347\205\247", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CameraWindow: public Ui_CameraWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERAWINDOW_H
