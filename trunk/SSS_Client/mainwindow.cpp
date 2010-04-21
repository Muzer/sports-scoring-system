#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->mainToolBar->setMovable(false);

	connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(showConnectDialog()));
	connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnect()));
	connect(ui->actionAdd_Table, SIGNAL(triggered()), this, SLOT(addEvent()));
	connect(ui->actionRemove_Table, SIGNAL(triggered()), this, SLOT(removeEvent()));

	socket = new QTcpSocket(this);
	connect(socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(disconnect()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError()));

	btnConnect = new QToolButton(this);
	btnConnect->setText("Connect");
	btnConnect->setIcon(QIcon(":/data/network-connect.png"));
	connect(btnConnect, SIGNAL(clicked()), this, SLOT(showConnectDialog()));
	ui->mainToolBar->addWidget(btnConnect);

	btnDisconnect = new QToolButton(this);
	btnDisconnect->setText("Disconnect");
	btnDisconnect->setEnabled(false);
	btnDisconnect->setIcon(QIcon(":/data/network-disconnect.png"));
	connect(btnDisconnect, SIGNAL(clicked()), this, SLOT(disconnect()));
	ui->mainToolBar->addWidget(btnDisconnect);

	ui->actionDisconnect->setEnabled(false);

	ui->mainToolBar->addSeparator();

	btnAddEvent = new QToolButton(this);
	btnAddEvent->setText("Add Event");
	btnAddEvent->setIcon(QIcon(":/data/archive-insert.png"));
	connect(btnAddEvent, SIGNAL(clicked()), this, SLOT(addEvent()));
	btnAddEvent->setEnabled(false);
	ui->mainToolBar->addWidget(btnAddEvent);

	btnRemoveEvent = new QToolButton(this);
	btnRemoveEvent->setText("Remove Event");
	btnRemoveEvent->setIcon(QIcon(":/data/archive-remove.png"));
	connect(btnRemoveEvent, SIGNAL(clicked()), this, SLOT(removeEvent()));
	btnRemoveEvent->setEnabled(false);
	ui->mainToolBar->addWidget(btnRemoveEvent);

	ui->menuActions->setEnabled(false);

	model = new QStandardItemModel(this);
	model->setColumnCount(2);
	model->setHorizontalHeaderLabels(QStringList() << "Year" << "Name");
	ui->treeView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	event->accept();
	ui->treeView->resize(ui->centralWidget->size());
}

void MainWindow::showConnectDialog()
{
	ConnectDialog *dialog = new ConnectDialog();
	if (dialog->exec() == QDialog::Accepted)
	{
		socket->connectToHost(dialog->getAddress(), dialog->getPort());
		username = dialog->getUsername();
		password = dialog->getPassword();
		btnConnect->setEnabled(false);
		btnDisconnect->setEnabled(true);
		btnAddEvent->setEnabled(true);
		btnRemoveEvent->setEnabled(true);

		ui->actionConnect->setEnabled(false);
		ui->actionDisconnect->setEnabled(true);
		ui->menuActions->setEnabled(true);
	}
}

void MainWindow::addEvent()
{
	AddEventDialog *dialog = new AddEventDialog(0, yeargroups);
	if (dialog->exec() == QDialog::Accepted)
	{

	}
}

void MainWindow::removeEvent()
{

}

void MainWindow::disconnect()
{
	btnConnect->setEnabled(true);
	btnDisconnect->setEnabled(false);
	btnAddEvent->setEnabled(false);
	btnRemoveEvent->setEnabled(false);

	ui->actionConnect->setEnabled(true);
	ui->actionDisconnect->setEnabled(false);
	ui->menuActions->setEnabled(false);

	socket->disconnectFromHost();
	setWindowTitle(QString("Sports Scoring System"));
}

void MainWindow::connected()
{
	QByteArray line = "AUTH ";
	line.append(username);
	line.append(";");
	line.append(password);
	line.append("\n");

	socket->write(line);
}

void MainWindow::readyRead()
{
	QByteArray data = socket->readLine(65563);
	QString string = data.data();

	cout << string.toStdString() << endl;

	if (string == "AUTHBAD\n")
	{
		disconnect();
		QMessageBox::critical(this, "Wrong Username or Password", "Either the username or password you entered was wrong.");
		showConnectDialog();
	}
	else if (string.startsWith("AUTHGOOD"))
	{
		QString name = string.remove(0, 9);
		name.remove("\n");

		setWindowTitle(QString("Sports Scoring System - ") + name);

		socket->write("AUTHDONE\n");
	}
	else if (string.startsWith("YEARGROUPS"))
	{
		QString temp = string.remove(0, 11);
		temp = temp.trimmed();
		yeargroups = temp.split(",");
	}
	else if (string == "CLEAREVENTLIST\n")
	{
		model->clear();
		model->setHorizontalHeaderLabels(QStringList() << "Year" << "Name");
	}
	else if (string.startsWith("ADDEVENT"))
	{
		QString temp = string.remove(0, 9);
		QStringList lists = temp.split(";");
		QString year = lists[0];
		QString event = lists[1];
		event = event.remove("\n");
		model->appendRow(QList<QStandardItem *>() << new QStandardItem(year) << new QStandardItem(event));
	}
}

void MainWindow::socketError()
{
	QMessageBox::warning(this, "Error", socket->errorString());
	disconnect();
}
