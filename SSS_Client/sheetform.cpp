#include "sheetform.h"
#include "ui_sheetform.h"

SheetForm::SheetForm(QWidget *parent, QString id, QTcpSocket *socket) :
    QWidget(parent),
    ui(new Ui::SheetForm)
{
    ui->setupUi(this);

	tcpSocket = socket;

	nameId = id;
	QString title = nameId.split("|")[0] + QString(" ") + nameId.split("|")[1];
	setWindowTitle(QString("Sports Scoring System - ") + title);

	model = new QStandardItemModel(this);

	ui->tableView->setModel(model);

	connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(sendAddRow()));
}

SheetForm::~SheetForm()
{
    delete ui;
	delete model;
}

QString SheetForm::getName()
{
	return nameId;
}

void SheetForm::addHeader(QString title)
{
	headers.append(title);
}

void SheetForm::setHeader()
{
	model->setColumnCount(headers.count());
	model->setHorizontalHeaderLabels(headers);
}

void SheetForm::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SheetForm::resizeEvent(QResizeEvent *event)
{
	event->accept();
	ui->verticalLayoutWidget->resize(this->size());
}

void SheetForm::sendAddRow()
{
	tcpSocket->write("ADDEVENTROW ");
	tcpSocket->write(nameId.toStdString().c_str());
	tcpSocket->write("\n");
	tcpSocket->flush();
}

void SheetForm::addRow()
{
	QList<QStandardItem *> list;
	for (int i = 0; i < headers.count(); i++)
	{
		list.append(new QStandardItem(""));
	}
	model->appendRow(list);
}
