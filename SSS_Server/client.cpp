#include <QCryptographicHash>
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

void Client::sendEvent(QString yeargroup, QString event)
{
	if (checkAuthentication())
	{
		tcpSocket->write("ADDEVENT ");
		tcpSocket->write(yeargroup.toStdString().c_str());
		tcpSocket->write(";");
		tcpSocket->write(event.toStdString().c_str());
		tcpSocket->write("\n");
		tcpSocket->flush();

		QString text2 = QString("SETEVENTFIELDS ") + toSssName(yeargroup) + QString(";") + toSssName(event) + QString(";");

		QSqlRecord record = database->record(toSqlName(yeargroup) + "|" + toSqlName(event));
		for (int i = 0; i < record.count(); i++)
		{
			text2 = text2.append(record.fieldName(i));
			if (i < record.count() - 1)
			{
				text2 = text2.append("|");
			}
		}

		text2 = text2.append("\n");
		tcpSocket->write(text2.toStdString().c_str());
		tcpSocket->flush();
	}
}

void Client::sendRemoveEvent(QString name)
{
	if (checkAuthentication())
	{
		tcpSocket->write("REMOVEEVENT ");
		tcpSocket->write(name.toStdString().c_str());
		tcpSocket->write("\n");
		tcpSocket->flush();
	}
}

void Client::sendAddEventRow(QString nameId)
{
	if (checkAuthentication())
	{
		tcpSocket->write("ADDEVENTROW ");
		tcpSocket->write(nameId.toStdString().c_str());
		tcpSocket->write("\n");
		tcpSocket->flush();
	}
}

void Client::writeAuthDone(QString string)
{
	QString str = "YEARGROUPS ";
	str.append(yeargroups);
	str.append("\n");
	tcpSocket->write(str.toStdString().c_str());
	tcpSocket->flush();
	tcpSocket->write("CLEAREVENTLIST\n");
	tcpSocket->flush();

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

		QString text2 = QString("SETEVENTFIELDS ") + year + QString(";") + event + QString(";");

		QSqlRecord record = database->record(toSqlName(year) + "|" + toSqlName(event));
		for (int i = 0; i < record.count(); i++)
		{
			text2 = text2.append(record.fieldName(i));
			if (i < record.count() - 1)
			{
				text2 = text2.append("|");
			}
		}

		text2 = text2.append("\n");
		tcpSocket->write(text2.toStdString().c_str());
		tcpSocket->flush();
	}

	tcpSocket->write("DONELOADING\n");
	tcpSocket->flush();
}

void Client::writeAuth(QString string)
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

void Client::writeAddEvent(QString string)
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

void Client::writeRemoveEvent(QString string)
{
	QString name = string.remove(0, 12);

	QString query = "DROP TABLE \"";
	query = query.append(name);
	query = query.append("\"");

	database->exec(query);

	emit removedEvent(name);
}

void Client::writeAddEventRow(QString string)
{
	QString nameId = string.remove(0, 12);

	QString tableName = toSqlName(nameId);
	QString query = "INSERT INTO \"";
	query = query.append(tableName);
	query = query.append("\" (");

	QSqlRecord record = database->record(tableName);
	for (int i = 0; i < record.count(); i++)
	{
		query = query.append("\"");
		query = query.append(record.fieldName(i));
		query = query.append("\"");

		if (i < record.count() - 1)
		{
			query.append(", ");
		}
	}

	query = query.append(") VALUES (");

	for (int i = 0; i < record.count(); i++)
	{
		query = query.append("\" \"");

		if (i < record.count() - 1)
		{
			query.append(", ");
		}
	}

	query = query.append(")");

	database->exec(query);

	cout << query.toStdString() << endl;
	cout << database->lastError().text().toStdString() << endl;

	emit addedEventRow(nameId);
}

QString Client::toSqlName(QString sssName)
{
	return sssName.replace(" ", "_");
}

QString Client::toSssName(QString sqlName)
{
	return sqlName.replace("_", " ");
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

		cout << string.toStdString() << endl;

		if (string == "AUTHDONE" && checkAuthentication())
		{
			writeAuthDone(string);
		}
		else if (string.startsWith("AUTH"))
		{
			writeAuth(string);
		}
		else if (string.startsWith("ADDEVENTROW") && checkAuthentication())
		{
			writeAddEventRow(string);
		}
		else if (string.startsWith("ADDEVENT") && checkAuthentication())
		{
			writeAddEvent(string);
		}
		else if (string.startsWith("REMOVEEVENT") && checkAuthentication())
		{
			writeRemoveEvent(string);
		}
		else
		{
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
