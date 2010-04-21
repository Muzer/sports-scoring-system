#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QStandardItemModel>
#include <iostream>

using namespace std;

namespace Ui {
    class AddEventDialog;
}

class AddEventDialog : public QDialog {
    Q_OBJECT
public:
	AddEventDialog(QWidget *parent, QStringList yeargroups);
    ~AddEventDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AddEventDialog *ui;
	QStandardItemModel *model;

private slots:
	void addSection();
	void removeSection();
	void moveUp();
	void moveDown();
};

#endif // ADDEVENTDIALOG_H
