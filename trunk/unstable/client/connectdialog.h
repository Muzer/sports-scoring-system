#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ConnectDialog;
}

class ConnectDialog : public QDialog {
    Q_OBJECT
public:
    ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ConnectDialog *m_ui;
};

#endif // CONNECTDIALOG_H
