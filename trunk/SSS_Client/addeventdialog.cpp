#include "addeventdialog.h"
#include "ui_addeventdialog.h"

AddEventDialog::AddEventDialog(QWidget *parent, QStringList yeargroups) :
    QDialog(parent),
    ui(new Ui::AddEventDialog)
{
    ui->setupUi(this);

	connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(addSection()));
	connect(ui->btnRemove, SIGNAL(clicked()), this, SLOT(removeSection()));
	connect(ui->btnUp, SIGNAL(clicked()), this, SLOT(moveUp()));
	connect(ui->btnDown, SIGNAL(clicked()), this, SLOT(moveDown()));

	model = new QStandardItemModel(this);
	ui->treeView->setModel(model);

	for (int i = 0; i < yeargroups.count(); i++)
	{
		ui->comboBox->addItem(yeargroups[i]);
	}
	ui->comboBox->addItem("All Yeargroups");
}

AddEventDialog::~AddEventDialog()
{
    delete ui;
}

void AddEventDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AddEventDialog::addSection()
{
	model->appendRow(QList<QStandardItem *>() << new QStandardItem("Event") << new QStandardItem("Type"));
}

void AddEventDialog::removeSection()
{
	int i = ui->treeView->currentIndex().row();
	if (i != -1)
	{
		model->removeRow(i);
	}
}

void AddEventDialog::moveUp()
{
	int i = ui->treeView->currentIndex().row();
	if (i > 0)
	{
		QStandardItem *item = new QStandardItem(model->item(i));
		model->removeRow(i);
		model->insertRow(i - 1, item);
	}
}

void AddEventDialog::moveDown()
{
	int i = ui->treeView->currentIndex().row();
	if (i != -1 && i < model->rowCount() - 1)
	{
		QStandardItem *item = new QStandardItem(model->item(i));
		model->removeRow(i);
		model->insertRow(i + 1, item);
	}
}
