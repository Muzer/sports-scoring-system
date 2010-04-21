#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QWidget>

namespace Ui {
    class ProgressDialog;
}

class ProgressDialog : public QWidget {
    Q_OBJECT
public:
    ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();

	void setValue(int i);
	void setText(QString text);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
