#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <iostream>
#include <sstream>

using namespace std;

class Client : public QObject
{
	Q_OBJECT
public:
	Client(int i, QTcpSocket *socket, QSqlDatabase *db, QString n, QString username, QString password, QString years);
	~Client();
	void sendEvent(QString yeargroup, QString event);
	void sendRemoveEvent(QString name);
	void sendAddEventRow(QString nameId);

	void writeAuthDone(QString string);
	void writeAuth(QString string);
	void writeAddEvent(QString string);
	void writeRemoveEvent(QString string);
	void writeAddEventRow(QString string);

	QString toSqlName(QString sssName);
	QString toSssName(QString sqlName);

	bool checkAuthentication();

private:
	QTcpSocket *tcpSocket;
	int id;
	bool authenticated;

	QString correctUsername;
	QString correctPassword;
	QString name;
	QString yeargroups;

	QString peerAddress;
	QString stringId;

	QSqlDatabase *database;

signals:
	void addedEvent(QString yeargroup, QString event);
	void removedEvent(QString name);

	void addedEventRow(QString name);

private slots:
	void disconnected();
	void readyRead();
};

void printColour(QString text, QString colour);

#endif // CLIENT_H
