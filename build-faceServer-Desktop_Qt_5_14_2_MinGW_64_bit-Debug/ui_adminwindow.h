/********************************************************************************
** Form generated from reading UI file 'adminwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINWINDOW_H
#define UI_ADMINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_adminwindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *backbt;
    QHBoxLayout *horizontalLayout_6;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLineEdit *oldphone;
    QHBoxLayout *horizontalLayout;
    QLineEdit *namele;
    QLineEdit *phonele;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *idcarle;
    QLineEdit *photoPathle;
    QPushButton *addbt;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *deletebt;
    QPushButton *findbt;
    QPushButton *updatabt;
    QHBoxLayout *horizontalLayout_4;
    QTableWidget *findInfmation;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *adminwindow)
    {
        if (adminwindow->objectName().isEmpty())
            adminwindow->setObjectName(QString::fromUtf8("adminwindow"));
        adminwindow->resize(778, 618);
        centralwidget = new QWidget(adminwindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_5->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        horizontalLayout_7->addLayout(horizontalLayout_5);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_3);

        backbt = new QPushButton(centralwidget);
        backbt->setObjectName(QString::fromUtf8("backbt"));

        horizontalLayout_7->addWidget(backbt);


        verticalLayout_3->addLayout(horizontalLayout_7);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));

        horizontalLayout_6->addWidget(listWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));

        horizontalLayout_6->addLayout(verticalLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        oldphone = new QLineEdit(centralwidget);
        oldphone->setObjectName(QString::fromUtf8("oldphone"));

        verticalLayout->addWidget(oldphone);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        namele = new QLineEdit(centralwidget);
        namele->setObjectName(QString::fromUtf8("namele"));

        horizontalLayout->addWidget(namele);

        phonele = new QLineEdit(centralwidget);
        phonele->setObjectName(QString::fromUtf8("phonele"));

        horizontalLayout->addWidget(phonele);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        idcarle = new QLineEdit(centralwidget);
        idcarle->setObjectName(QString::fromUtf8("idcarle"));

        horizontalLayout_2->addWidget(idcarle);

        photoPathle = new QLineEdit(centralwidget);
        photoPathle->setObjectName(QString::fromUtf8("photoPathle"));

        horizontalLayout_2->addWidget(photoPathle);


        verticalLayout->addLayout(horizontalLayout_2);

        addbt = new QPushButton(centralwidget);
        addbt->setObjectName(QString::fromUtf8("addbt"));

        verticalLayout->addWidget(addbt);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        deletebt = new QPushButton(centralwidget);
        deletebt->setObjectName(QString::fromUtf8("deletebt"));

        horizontalLayout_3->addWidget(deletebt);


        verticalLayout->addLayout(horizontalLayout_3);

        findbt = new QPushButton(centralwidget);
        findbt->setObjectName(QString::fromUtf8("findbt"));

        verticalLayout->addWidget(findbt);

        updatabt = new QPushButton(centralwidget);
        updatabt->setObjectName(QString::fromUtf8("updatabt"));

        verticalLayout->addWidget(updatabt);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        findInfmation = new QTableWidget(centralwidget);
        findInfmation->setObjectName(QString::fromUtf8("findInfmation"));

        horizontalLayout_4->addWidget(findInfmation);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_6->addLayout(verticalLayout);


        verticalLayout_3->addLayout(horizontalLayout_6);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 1);

        adminwindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(adminwindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 778, 23));
        adminwindow->setMenuBar(menubar);
        statusbar = new QStatusBar(adminwindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        adminwindow->setStatusBar(statusbar);

        retranslateUi(adminwindow);

        QMetaObject::connectSlotsByName(adminwindow);
    } // setupUi

    void retranslateUi(QMainWindow *adminwindow)
    {
        adminwindow->setWindowTitle(QCoreApplication::translate("adminwindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("adminwindow", "\345\221\230\345\267\245\344\277\241\346\201\257\345\210\227\350\241\250", nullptr));
        backbt->setText(QCoreApplication::translate("adminwindow", "\350\277\224\345\233\236", nullptr));
        oldphone->setText(QString());
        oldphone->setPlaceholderText(QCoreApplication::translate("adminwindow", "\344\277\256\346\224\271\345\221\230\345\267\245\344\277\241\346\201\257\350\257\267\345\205\210\350\276\223\345\205\245\346\211\213\346\234\272\345\217\267", nullptr));
        namele->setPlaceholderText(QCoreApplication::translate("adminwindow", "\345\221\230\345\267\245\345\247\223\345\220\215", nullptr));
        phonele->setPlaceholderText(QCoreApplication::translate("adminwindow", "\345\221\230\345\267\245\346\211\213\346\234\272\345\217\267", nullptr));
        idcarle->setPlaceholderText(QCoreApplication::translate("adminwindow", "\345\221\230\345\267\245\350\272\253\344\273\275\350\257\201\345\217\267", nullptr));
        photoPathle->setPlaceholderText(QCoreApplication::translate("adminwindow", "\345\221\230\345\267\245\347\205\247\347\211\207\350\267\257\345\276\204", nullptr));
        addbt->setText(QCoreApplication::translate("adminwindow", "\346\267\273\345\212\240\345\221\230\345\267\245", nullptr));
        deletebt->setText(QCoreApplication::translate("adminwindow", "\345\210\240\351\231\244\345\221\230\345\267\245", nullptr));
        findbt->setText(QCoreApplication::translate("adminwindow", "\346\237\245\350\257\242\345\221\230\345\267\245", nullptr));
        updatabt->setText(QCoreApplication::translate("adminwindow", "\344\277\256\346\224\271\345\221\230\345\267\245\344\277\241\346\201\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class adminwindow: public Ui_adminwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINWINDOW_H
