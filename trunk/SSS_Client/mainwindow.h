#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QTcpSocket>
#include <QMessageBox>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <iostream>

#include "connectdialog.h"
#include "addeventdialog.h"
#include "progressdialog.h"

using namespace std;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

private slots:
	void showConnectDialog();
	void addEvent();
	void removeEvent();

	void connected();
	void disconnect();
	void readyRead();
	void socketError();
};

#endif // MAINWINDOW_H
