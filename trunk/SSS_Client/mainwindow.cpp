#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "addeventdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->mainToolBar->setMovable(false);

	progressDialog = new ProgressDialog(0);

	connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(showConnectDialog()));
	connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnect()));
	connect(ui->actionAdd_Table, SIGNAL(triggered()), this, SLOT(addEvent()));
	connect(ui->actionRemove_Table, SIGNAL(triggered()), this, SLOT(removeEvent()));

	connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showSheetForm()));

	ui->treeView->setEditTriggers(QTreeView::NoEditTriggers);

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
	model->setHorizontalHeaderLabels(QStringList() << "Year" << "Event");
	ui->treeView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::toSqlName(QString sssName)
{
	return sssName.replace(" ", "_");
}

QString MainWindow::toSssName(QString sqlName)
{
	return sqlName.replace("_", " ");
}

void MainWindow::writeNewEvent(QString yeargroup, QString eventname, AddEventDialog *dialog)
{
	socket->write("ADDEVENT ");
	socket->write(yeargroup.toStdString().c_str());
	socket->write(";");
	socket->write(eventname.toStdString().c_str());
	socket->write(";");

	for (int i = 0; i < dialog->rows(); i++)
	{
		socket->write(dialog->getModelText(i, 0).toStdString().c_str());
		socket->write("|");
		socket->write(dialog->getModelText(i, 1).toStdString().c_str());
		if (i < dialog->rows() - 1)
		{
			socket->write(",");
		}
	}

	socket->write("\n");
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

	delete dialog;
}

void MainWindow::addEvent()
{
	AddEventDialog *dialog = new AddEventDialog(0, yeargroups);
	if (dialog->exec() == QDialog::Accepted)
	{
		QString yeargroup = dialog->getYeargroup();
		QString eventname = dialog->getEventName();

		if (yeargroup.startsWith("All"))
		{
			for (int i = 0; i < yeargroups.count(); i++)
			{
				writeNewEvent(yeargroups[i], eventname, dialog);
			}
		}
		else
		{
			writeNewEvent(yeargroup, eventname, dialog);
		}
	}

	delete dialog;
}

void MainWindow::removeEvent()
{
	int i = ui->treeView->currentIndex().row();
	if (i >= 0)
	{
		if (QMessageBox::question(this, "Are you sure?", "Are you sure you want to delete this event. This action is permenant and all data will be lost.", QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
		{
			QString name = model->item(i, 0)->text().replace(" ", "_");
			name = name.append("|");
			name = name.append(model->item(i, 1)->text().replace(" ", "_"));
			socket->write("REMOVEEVENT ");
			socket->write(name.toStdString().c_str());
			socket->write("\n");
			socket->flush();
		}
	}
}

void MainWindow::showSheetForm()
{
	int i = ui->treeView->currentIndex().row();
	if (i >= 0)
	{
		QString name = model->item(i, 0)->text() + "|" + model->item(i, 1)->text();
		for (int i = 0; i < sheetForms.count(); i++)
		{
			if (sheetForms[i]->getName() == name)
			{
				sheetForms[i]->show();
			}
		}
	}
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

	model->clear();
	model->setHorizontalHeaderLabels(QStringList() << "Year" << "Event");

	for (int i = 0; i < sheetForms.count(); i++)
	{
		delete sheetForms[i];
	}
	sheetForms.clear();

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
	while (socket->canReadLine())
	{
		QByteArray data = socket->readLine(65563);
		QString string = data.data();
		string = string.trimmed();

		cout << string.toStdString() << endl;

		if (string == "AUTHBAD")
		{
			disconnect();
			QMessageBox::critical(this, "Wrong Username or Password", "Either the username or password you entered was wrong.");
			showConnectDialog();
		}
		else if (string.startsWith("AUTHGOOD"))
		{
			QString name = string.remove(0, 9);

			setWindowTitle(QString("Sports Scoring System - ") + name);

			socket->write("AUTHDONE\n");
			progressDialog->setValue(0);
			progressDialog->setText("Loading current SSS status...");
			progressDialog->show();
		}
		else if (string.startsWith("YEARGROUPS"))
		{
			QString temp = string.remove(0, 11);
			yeargroups = temp.split(",");
			progressDialog->setText("Getting year groups...");
			progressDialog->setValue(2);
		}
		else if (string == "CLEAREVENTLIST")
		{
			model->clear();
			model->setHorizontalHeaderLabels(QStringList() << "Year" << "Event");
			progressDialog->setText("Clearing Event List...");
			progressDialog->setValue(4);
		}
		else if (string.startsWith("ADDEVENTROW"))
		{
			QString nameId = string.remove(0, 12);
			for (int i = 0; i < sheetForms.count(); i++)
			{
				if (sheetForms[i]->getName() == nameId)
				{
					sheetForms[i]->addRow();
				}
			}
		}
		else if (string.startsWith("ADDEVENT"))
		{
			QString temp = string.remove(0, 9);
			QStringList lists = temp.split(";");
			QString year = lists[0];
			QString event = lists[1];
			model->appendRow(QList<QStandardItem *>() << new QStandardItem(year) << new QStandardItem(event));

			sheetForms.append(new SheetForm(0, year + "|" + event, socket));

			progressDialog->setText("Adding events...");
			progressDialog->setValue(10);
		}
		else if (string.startsWith("REMOVEEVENT"))
		{
			QString name = string.remove(0, 12);
			QString name1 = name.split("|")[0];
			name1 = name1.replace("_", " ");
			QString name2 = name.split("|")[1];
			name2 = name2.replace("_", " ");

			for (int i = model->rowCount() - 1; i >= 0; i--)
			{
				if (model->item(i, 0)->text() == name1)
				{
					if (model->item(i, 1)->text() == name2)
					{
						model->removeRow(i);
					}
				}
			}
		}
		else if (string.startsWith("SETEVENTFIELDS"))
		{
			QStringList sections = string.remove(0, 15).split(";");
			QStringList fields = sections[2].split("|");
			for (int j = 0; j < sheetForms.count(); j++)
			{
				QString name = toSssName(sections[0]) + "|" + toSssName(sections[1]);
				if (sheetForms[j]->getName() == name)
				{
					for (int i = 0; i < fields.count(); i++)
					{
						sheetForms[j]->addHeader(toSssName(fields[i]));
					}
					sheetForms[j]->setHeader();
				}
			}
		}
		else if (string.startsWith("ERROR"))
		{
			QString message = string.remove(0, 6);
			QMessageBox::critical(this, "Error", message);
		}
		else if (string == "DONELOADING")
		{
			progressDialog->setValue(100);
			progressDialog->setText("Complete!");
			progressDialog->hide();
		}
	}
}

void MainWindow::socketError()
{
	QMessageBox::warning(this, "Error", socket->errorString());
	disconnect();
}
