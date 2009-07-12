#ifndef NEWTABLEWIZARD_H
#define NEWTABLEWIZARD_H

#include <QtGui/QWizard>

namespace Ui {
    class newtablewizard;
}

class newtablewizard : public QWizard {
    Q_OBJECT
public:
    newtablewizard(QWidget *parent = 0);
    ~newtablewizard();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::newtablewizard *m_ui;

private slots:
    void changeNoListItems(int a);

};

#endif // NEWTABLEWIZARD_H
