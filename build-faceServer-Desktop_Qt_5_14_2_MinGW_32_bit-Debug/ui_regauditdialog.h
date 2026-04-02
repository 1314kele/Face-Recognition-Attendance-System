/********************************************************************************
** Form generated from reading UI file 'regauditdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGAUDITDIALOG_H
#define UI_REGAUDITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_regauditdialog
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *aggreBt;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_2;

    void setupUi(QDialog *regauditdialog)
    {
        if (regauditdialog->objectName().isEmpty())
            regauditdialog->setObjectName(QString::fromUtf8("regauditdialog"));
        regauditdialog->resize(400, 300);
        gridLayout = new QGridLayout(regauditdialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        label = new QLabel(regauditdialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        textEdit = new QTextEdit(regauditdialog);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        verticalLayout->addWidget(textEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        aggreBt = new QPushButton(regauditdialog);
        aggreBt->setObjectName(QString::fromUtf8("aggreBt"));

        horizontalLayout->addWidget(aggreBt);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_2 = new QPushButton(regauditdialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);


        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(regauditdialog);

        QMetaObject::connectSlotsByName(regauditdialog);
    } // setupUi

    void retranslateUi(QDialog *regauditdialog)
    {
        regauditdialog->setWindowTitle(QCoreApplication::translate("regauditdialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("regauditdialog", "\346\230\257\345\220\246\345\220\214\346\204\217\345\221\230\345\267\245\346\263\250\345\206\214", nullptr));
        aggreBt->setText(QCoreApplication::translate("regauditdialog", "\345\220\214\346\204\217", nullptr));
        pushButton_2->setText(QCoreApplication::translate("regauditdialog", "\344\270\215\345\220\214\346\204\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class regauditdialog: public Ui_regauditdialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGAUDITDIALOG_H
