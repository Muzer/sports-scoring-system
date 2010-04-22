#ifndef SHEETFORM_H
#define SHEETFORM_H

#include <QWidget>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QTcpSocket>
#include <iostream>

using namespace std;

namespace Ui {
    class SheetForm;
}

class SheetForm : public QWidget {
    Q_OBJECT
public:
	SheetForm(QWidget *parent, QString id, QTcpSocket *socket);
    ~SheetForm();

	QString getName();
	void addHeader(QString title);
	void setHeader();

	void addRow();

protected:
    void changeEvent(QEvent *e);
	void resizeEvent(QResizeEvent *event);

private:
    Ui::SheetForm *ui;
	QString nameId;
	QStandardItemModel *model;
	QStringList headers;
	QTcpSocket *tcpSocket;

public slots:
	void sendAddRow();
};

#endif // SHEETFORM_H
