#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QStandardItemModel>
#include <iostream>
#include <QMessageBox>
#include <sstream>

using namespace std;

namespace Ui {
    class AddEventDialog;
}

class AddEventDialog : public QDialog {
    Q_OBJECT
public:
	AddEventDialog(QWidget *parent, QStringList yeargroups);
    ~AddEventDialog();

	QString getYeargroup();
	QString getEventName();
	QString getModelText(int row, int column);

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

	void ok();
};

#endif // ADDEVENTDIALOG_H
