#ifndef NEWTABLEDIALOG_H
#define NEWTABLEDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class NewTableDialog;
}

class NewTableDialog : public QDialog {
    Q_OBJECT
public:
    NewTableDialog(QWidget *parent = 0);
    ~NewTableDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::NewTableDialog *m_ui;
};

#endif // NEWTABLEDIALOG_H
