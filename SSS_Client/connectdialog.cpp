#include "connectdialog.h"
#include "ui_connectdialog.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

QString ConnectDialog::getUsername()
{
	return ui->lineUsername->text();
}

QString ConnectDialog::getPassword()
{
	return ui->linePassword->text();
}

int ConnectDialog::getPort()
{
	return ui->linePort->text().toInt(0, 10);
}

QString ConnectDialog::getAddress()
{
	return ui->lineAddress->text();
}

void ConnectDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
