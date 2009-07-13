#include "newtablewizard.h"
#include "ui_newtablewizard.h"
#include <iostream>

newtablewizard::newtablewizard(QWidget *parent) :
    QWizard(parent),
    m_ui(new Ui::newtablewizard)
{
    m_ui->setupUi(this);
}

newtablewizard::~newtablewizard()
{
    delete m_ui;
}

void newtablewizard::addItem()
{
    m_ui->listWidget->addItem(m_ui->lineEdit->text());
}

void newtablewizard::remItem()
{
    std::cout << "hi\n";
    m_ui->listWidget->removeItemWidget(m_ui->listWidget->item(m_ui->listWidget->currentRow()));
}

void newtablewizard::changeEvent(QEvent *e)
{
    QWizard::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
