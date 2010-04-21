#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <iostream>
#include <QFile>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "client.h"

using namespace std;

class Server : public QObject
{
	Q_OBJECT
public:
    Server();
	~Server();

	void start();

private:
	QTcpServer *tcpServer;
	QList<Client *> clients;
	QString name;
	QString username;
	QString password;
	QString yeargroups;

	QSqlDatabase database;

private slots:
	void newConnection();
	void addedEvent(QString yeargroup, QString event);
	void removedEvent(QString name);
};

#endif // SERVER_H
