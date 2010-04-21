#include "server.h"

Server::Server()
{
	database = QSqlDatabase::addDatabase("QSQLITE");
	tcpServer = new QTcpServer(this);
	connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

Server::~Server()
{
	tcpServer->close();
	delete tcpServer;

	for (int i = clients.count() - 1; i >= 0; i--)
	{
		delete clients[i];
		clients.removeAt(i);
	}
}

void Server::start()
{
	printColour(":: Starting Server...", "blue");

	bool fine = false;

	name = "SSS";
	username = "admin";
	password = "password";
	yeargroups = "Year 7,Year 8,Year 9,Year 10,Year 11,Year 12,Year 13";

	QFile fp("config.ini");
	if (fp.open(QIODevice::Text | QIODevice::ReadOnly))
	{
		fine = true;
		bool working = false;
		while (!fp.atEnd())
		{
			QByteArray line = fp.readLine();
			if (line == "[SSSConfig]\n")
			{
				working = true;
			}

			if (working)
			{
				QString key = string(line.data()).substr(0, line.indexOf("=")).c_str();
				QString value = string(line.data()).substr(line.indexOf("=") + 1, line.length()).c_str();

				if (key.toLower() == "name")
				{
					name = value.trimmed();
				}
				else if (key.toLower() == "username")
				{
					username = value.trimmed();
				}
				else if (key.toLower() == "password")
				{
					password = value.trimmed();
				}
				else if (key.toLower() == "yeargroups")
				{
					yeargroups = value.trimmed();
				}
			}
		}
	}
	else
	{
		printColour(QString(":: Could not open \"config.ini\": ") + fp.errorString(), "red");
	}

	database.setDatabaseName("database.db");

	if (database.open())
	{
		fine = true;
	}
	else
	{
		printColour(QString(":: Could not open \"database.db\": ") + database.lastError().text(), "red");
		fine = false;
	}

	if (fine)
	{
		tcpServer->listen(QHostAddress::LocalHost, 8643);

		if (name == "SSS")
		{
			printColour(":: Warning: Using default name, \"" + name + "\", as none is set.", "red");
		}
		if (username == "admin")
		{
			printColour(":: Warning: Using default username, \"" + username + "\", as none is set.", "red");
		}
		if (password == "password")
		{
			printColour(":: Warning: Using default password, \"" + password + "\", as none is set.", "red");
		}
		if (yeargroups == "Year 7,Year 8,Year 9,Year 10,Year 11,Year 12,Year 13")
		{
			printColour(":: Warning: Using default yeargroups, \"" + yeargroups + "\", as none is set.", "red");
		}
	}
	else
	{
		exit(0);
	}

	printColour(":: Server Started Successfully!", "blue");
}

void Server::newConnection()
{
	while (tcpServer->hasPendingConnections())
	{
		printColour(":: New Connection...", "blue");
		QTcpSocket *socket = tcpServer->nextPendingConnection();
		Client *client = new Client(clients.count(), socket, &database, name, username, password, yeargroups);
		clients.append(client);
	}
}