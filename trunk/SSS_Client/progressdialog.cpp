#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::setValue(int i)
{
	ui->progressBar->setValue(i);
}

void ProgressDialog::setText(QString text)
{
	ui->label->setText(text);
}

void ProgressDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
