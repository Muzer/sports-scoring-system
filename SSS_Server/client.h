#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QSqlDatabase>
#include <iostream>
#include <sstream>

using namespace std;

class Client : public QObject
{
	Q_OBJECT
public:
	Client(int i, QTcpSocket *socket, QSqlDatabase *db, QString n, QString username, QString password, QString years);
	~Client();

private:
	QTcpSocket *tcpSocket;
	int id;

	QString correctUsername;
	QString correctPassword;
	QString name;
	QString yeargroups;

	QSqlDatabase *database;

private slots:
	void disconnected();
	void readyRead();
};

void printColour(QString text, QString colour);

#endif // CLIENT_H
