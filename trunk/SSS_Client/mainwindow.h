#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QTcpSocket>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <iostream>

#include "addeventdialog.h"
#include "progressdialog.h"
#include "sheetform.h"

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

	QString toSqlName(QString sssName);
	QString toSssName(QString sqlName);

	void writeNewEvent(QString yeargroup, QString eventname, AddEventDialog *dialog);

protected:
    void changeEvent(QEvent *e);
	void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;
	QToolButton *btnConnect;
	QToolButton *btnDisconnect;
	QToolButton *btnAddEvent;
	QToolButton *btnRemoveEvent;

	QTcpSocket *socket;

	QString username;
	QString password;
	QStringList yeargroups;

	QStandardItemModel *model;

	ProgressDialog *progressDialog;

	QList<SheetForm *> sheetForms;

private slots:
	void showConnectDialog();
	void addEvent();
	void removeEvent();

	void showSheetForm();

	void connected();
	void disconnect();
	void readyRead();
	void socketError();
};

#endif // MAINWINDOW_H
