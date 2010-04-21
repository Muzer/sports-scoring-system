/********************************************************************************
** Form generated from reading UI file 'addeventdialog.ui'
**
** Created: Wed 21. Apr 12:14:54 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDEVENTDIALOG_H
#define UI_ADDEVENTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddEventDialog
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *comboBox;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QTreeView *treeView;
    QPushButton *btnRemove;
    QPushButton *btnAdd;
    QPushButton *btnUp;
    QPushButton *btnDown;

    void setupUi(QDialog *AddEventDialog)
    {
        if (AddEventDialog->objectName().isEmpty())
            AddEventDialog->setObjectName(QString::fromUtf8("AddEventDialog"));
        AddEventDialog->resize(240, 320);
        AddEventDialog->setMinimumSize(QSize(240, 320));
        AddEventDialog->setMaximumSize(QSize(240, 320));
        buttonBox = new QDialogButtonBox(AddEventDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 280, 221, 31));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        formLayoutWidget = new QWidget(AddEventDialog);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 221, 231));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        comboBox = new QComboBox(formLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setEditable(false);

        formLayout->setWidget(0, QFormLayout::FieldRole, comboBox);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        lineEdit = new QLineEdit(formLayoutWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, lineEdit);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        treeView = new QTreeView(formLayoutWidget);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        formLayout->setWidget(2, QFormLayout::FieldRole, treeView);

        btnRemove = new QPushButton(AddEventDialog);
        btnRemove->setObjectName(QString::fromUtf8("btnRemove"));
        btnRemove->setGeometry(QRect(200, 250, 31, 24));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/data/list-remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnRemove->setIcon(icon);
        btnAdd = new QPushButton(AddEventDialog);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));
        btnAdd->setGeometry(QRect(160, 250, 31, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/data/list-add.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnAdd->setIcon(icon1);
        btnUp = new QPushButton(AddEventDialog);
        btnUp->setObjectName(QString::fromUtf8("btnUp"));
        btnUp->setGeometry(QRect(120, 250, 31, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/data/go-up.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnUp->setIcon(icon2);
        btnDown = new QPushButton(AddEventDialog);
        btnDown->setObjectName(QString::fromUtf8("btnDown"));
        btnDown->setGeometry(QRect(80, 250, 31, 24));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/data/go-down.png"), QSize(), QIcon::Normal, QIcon::Off);
        btnDown->setIcon(icon3);

        retranslateUi(AddEventDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AddEventDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AddEventDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AddEventDialog);
    } // setupUi

    void retranslateUi(QDialog *AddEventDialog)
    {
        AddEventDialog->setWindowTitle(QApplication::translate("AddEventDialog", "Sports Scoring System - Add Event", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AddEventDialog", "Yeargroups: ", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("AddEventDialog", "Event: ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("AddEventDialog", "Sections: ", 0, QApplication::UnicodeUTF8));
        btnRemove->setText(QString());
        btnAdd->setText(QString());
        btnUp->setText(QString());
        btnDown->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AddEventDialog: public Ui_AddEventDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDEVENTDIALOG_H
