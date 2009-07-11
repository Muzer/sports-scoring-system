#include "connectdialog.h"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ConnectDialog)
{
    m_ui->setupUi(this);
}

ConnectDialog::~ConnectDialog()
{
    delete m_ui;
}

void ConnectDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString ConnectDialog::getLineEditText()
{
    return m_ui->lineEdit->text();
}

QString ConnectDialog::getLineEdit2Text()
{
    return m_ui->lineEdit_2->text();
}

QString ConnectDialog::getLineEdit3Text()
{
    return m_ui->lineEdit_3->text();
}
