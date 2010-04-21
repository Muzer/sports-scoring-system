#include "client.h"

Client::Client(int i, QTcpSocket *socket, QSqlDatabase *db, QString n, QString username, QString password, QString years)
{
	tcpSocket = socket;
	id = i;
	name = n;
	database = db;
	correctUsername = username;
	correctPassword = password;
	yeargroups = years;

	connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

Client::~Client()
{
	tcpSocket->disconnectFromHost();
	delete tcpSocket;
}

void Client::disconnected()
{
	stringstream ss;
	ss << ":: Client " << id << " Disconnected.";
	printColour(ss.str().c_str(), "blue");
}

void Client::readyRead()
{
	QByteArray data = tcpSocket->readLine(65563);
	QString string = data.data();

	if (string == "AUTHDONE\n")
	{
		QString string = "YEARGROUPS ";
		string.append(yeargroups);
		string.append("\n");
		tcpSocket->write(string.toStdString().c_str());
		tcpSocket->flush();
		tcpSocket->write("CLEAREVENTLIST\n");
		tcpSocket->flush();

		QStringList list = database->tables();
		for (int i = 0; i < list.count(); i++)
		{
			QString year = list[i].split("@")[0];
			QString event = list[i].split("@")[1];

			year = year.replace("_", " ");
			event = event.replace("_", " ");

			QString text = QString("ADDEVENT ") + year + QString(";") + event;
			tcpSocket->write(text.toStdString().c_str());
			tcpSocket->flush();
		}
	}
	else if (string.startsWith("AUTH"))
	{
		QString temp = string.remove(0, 5);
		QStringList lists = temp.split(";");
		QString username = lists[0];
		QString password = lists[1];
		password = password.remove("\n");

		if (username == correctUsername && password == correctPassword)
		{
			QString n = "AUTHGOOD ";
			n.append(name);
			n.append("\n");
			tcpSocket->write(n.toStdString().c_str());
		}
		else
		{
			tcpSocket->write("AUTHBAD\n");
		}
	}
	else
	{
		stringstream ss;
		ss << ":: " << "[" << id << "] " << data.data();
		printColour(ss.str().c_str(), "green");
	}
}

void printColour(QString text, QString colour)
{
#ifndef WIN32
	if (colour == "blue")
	{
		cout << "\033[94m";
	}
	else if (colour == "red")
	{
		cout << "\033[91m";
	}
	else if (colour == "green")
	{
		cout << "\033[92m";
	}
#endif

	cout << text.toStdString();

#ifdef WIN32
	cout << endl;
#else
	cout << "\033[0m" << endl;
#endif
}
