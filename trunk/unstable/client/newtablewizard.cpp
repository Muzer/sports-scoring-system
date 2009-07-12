#include "newtablewizard.h"
#include "ui_newtablewizard.h"

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

void newtablewizard::changeNoListItems(int a)
{
    int b, d, i;
    b = m_ui->listWidget->count();
    if (not a == b) {
        d = a - b;
        if(d < 0){
            d = 0 - d;
            for(i=0;i!=d;++i){
                m_ui->listWidget->removeItemWidget(m_ui->listWidget->currentItem());
            }
        }
        else if(d > 0){
            for(i=0;i!=d;++i){
                m_ui->listWidget->addItem("New Group");
            }
        }

    }
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
