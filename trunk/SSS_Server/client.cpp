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
	authenticated = false;

	peerAddress = tcpSocket->peerAddress().toString();
	stringstream ss;
	ss << "Client [" << id << "]:" << peerAddress.toStdString() << ")";
	stringId = ss.str().c_str();

	connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

Client::~Client()
{
	tcpSocket->disconnectFromHost();
	delete tcpSocket;
}

void Client::writeEvent(QString yeargroup, QString event)
{
	if (checkAuthentication())
	{
		tcpSocket->write("ADDEVENT ");
		tcpSocket->write(yeargroup.toStdString().c_str());
		tcpSocket->write(";");
		tcpSocket->write(event.toStdString().c_str());
		tcpSocket->write("\n");
		tcpSocket->flush();
	}
}

void Client::writeRemoveEvent(QString name)
{
	if (checkAuthentication())
	{
		tcpSocket->write("REMOVEEVENT ");
		tcpSocket->write(name.toStdString().c_str());
		tcpSocket->write("\n");
		tcpSocket->flush();
	}
}

bool Client::checkAuthentication()
{
	if (!authenticated)
	{
		printColour(QString(":: ") + stringId + QString(" is trying to do something without authenticating."), "red");
		tcpSocket->disconnectFromHost();
	}
	return authenticated;
}

void Client::disconnected()
{
	stringstream ss;
	printColour(QString(":: ") + stringId + QString(" disconnected."), "blue");
}

void Client::readyRead()
{
	while (tcpSocket->canReadLine())
	{
		QByteArray data = tcpSocket->readLine(65563);
		QString string = data.data();
		string = string.trimmed();

		if (string == "AUTHDONE" && checkAuthentication())
		{
			QString string = "YEARGROUPS ";
			string.append(yeargroups);
			string.append("\n");
			tcpSocket->write(string.toStdString().c_str());
			tcpSocket->flush();
			tcpSocket->waitForBytesWritten();
			tcpSocket->write("CLEAREVENTLIST\n");
			tcpSocket->flush();
			tcpSocket->waitForBytesWritten();

			QStringList list = database->tables();
			for (int i = 0; i < list.count(); i++)
			{
				QString year = list[i].split("|")[0];
				QString event = list[i].split("|")[1];

				year = year.replace("_", " ");
				event = event.replace("_", " ");

				QString text = QString("ADDEVENT ") + year + QString(";") + event + QString("\n");
				tcpSocket->write(text.toStdString().c_str());
				tcpSocket->flush();
			}

			tcpSocket->write("DONELOADING\n");
			tcpSocket->flush();
			tcpSocket->waitForBytesWritten();
		}
		else if (string.startsWith("AUTH"))
		{
			QString temp = string.remove(0, 5);
			QStringList lists = temp.split(";");
			QString username = lists[0];
			QString password = lists[1];

			if (username == correctUsername && password == correctPassword)
			{
				QString n = "AUTHGOOD ";
				n.append(name);
				n.append("\n");
				tcpSocket->write(n.toStdString().c_str());

				authenticated = true;
			}
			else
			{
				tcpSocket->write("AUTHBAD\n");
				tcpSocket->disconnectFromHost();

				authenticated = false;
			}
		}
		else if (string.startsWith("ADDEVENT") && checkAuthentication())
		{
			QString temp = string.remove(0, 9);
			QStringList lists = temp.split(";");
			QString yeargroup = lists[0];
			QString name = lists[1];
			QString sqlName = yeargroup.replace(" ", "_");
			sqlName = sqlName.append("|");
			sqlName = sqlName.append(name.replace(" ", "_"));

			QStringList sections = lists[2].split(",");

			QString query = "CREATE TABLE \"";
			query = query.append(sqlName);
			query = query.append("\"");
			query = query.append(" (");

			for (int i = 0; i < sections.count(); i++)
			{
				QString n = sections[i].split("|")[0].replace(" ", "_");
				QString t = sections[i].split("|")[1];

				if (t == "TIME")
				{
					n = QString("time_") + n;
				}

				query = query.append(n);
				query = query.append(" TEXT");

				if (i < sections.count() - 1)
				{
					query = query.append(",");
				}
			}

			query = query.append(")");

			database->exec(query);

			emit addedEvent(yeargroup.replace("_", " "), name.replace("_", " "));
		}
		else if (string.startsWith("REMOVEEVENT") && checkAuthentication())
		{
			QString name = string.remove(0, 12);

			QString query = "DROP TABLE \"";
			query = query.append(name);
			query = query.append("\"");

			database->exec(query);

			emit removedEvent(name);
		}
		else
		{
			stringstream ss;
			printColour(QString(":: ") + stringId + QString(" said \"") + string + QString("\""), "blue");
		}
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
