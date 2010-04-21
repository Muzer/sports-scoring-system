/********************************************************************************
** Form generated from reading UI file 'connectdialog.ui'
**
** Created: Tue Apr 20 18:21:48 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTDIALOG_H
#define UI_CONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConnectDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineAddress;
    QLineEdit *linePort;
    QLineEdit *lineUsername;
    QLineEdit *linePassword;

    void setupUi(QDialog *ConnectDialog)
    {
        if (ConnectDialog->objectName().isEmpty())
            ConnectDialog->setObjectName(QString::fromUtf8("ConnectDialog"));
        ConnectDialog->resize(400, 156);
        ConnectDialog->setMinimumSize(QSize(400, 156));
        ConnectDialog->setMaximumSize(QSize(400, 156));
        buttonBox = new QDialogButtonBox(ConnectDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 120, 381, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        formLayoutWidget = new QWidget(ConnectDialog);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 381, 114));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_4);

        lineAddress = new QLineEdit(formLayoutWidget);
        lineAddress->setObjectName(QString::fromUtf8("lineAddress"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineAddress);

        linePort = new QLineEdit(formLayoutWidget);
        linePort->setObjectName(QString::fromUtf8("linePort"));

        formLayout->setWidget(1, QFormLayout::FieldRole, linePort);

        lineUsername = new QLineEdit(formLayoutWidget);
        lineUsername->setObjectName(QString::fromUtf8("lineUsername"));

        formLayout->setWidget(2, QFormLayout::FieldRole, lineUsername);

        linePassword = new QLineEdit(formLayoutWidget);
        linePassword->setObjectName(QString::fromUtf8("linePassword"));
        linePassword->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(3, QFormLayout::FieldRole, linePassword);


        retranslateUi(ConnectDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConnectDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConnectDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectDialog)
    {
        ConnectDialog->setWindowTitle(QApplication::translate("ConnectDialog", "Sports Scoring System - Connect to Server", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ConnectDialog", "Address: ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ConnectDialog", "Port: ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ConnectDialog", "Username: ", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ConnectDialog", "Password: ", 0, QApplication::UnicodeUTF8));
        lineAddress->setText(QApplication::translate("ConnectDialog", "localhost", 0, QApplication::UnicodeUTF8));
        linePort->setText(QApplication::translate("ConnectDialog", "8643", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConnectDialog: public Ui_ConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDIALOG_H
