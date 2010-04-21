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
		QString text = model->item(i, 0)->text();
		QString text2 = model->item(i, 1)->text();
		QString nText = model->item(i - 1, 0)->text();
		QString nText2 = model->item(i - 1, 1)->text();

		model->item(i, 0)->setText(nText);
		model->item(i, 1)->setText(nText2);

		model->item(i - 1, 0)->setText(text);
		model->item(i - 1, 1)->setText(text2);

		ui->treeView->setCurrentIndex(model->index(i - 1, 0));
	}
}

void AddEventDialog::moveDown()
{
	int i = ui->treeView->currentIndex().row();
	if (i != -1 && i < model->rowCount() - 1)
	{
		QString text = model->item(i, 0)->text();
		QString text2 = model->item(i, 1)->text();
		QString nText = model->item(i + 1, 0)->text();
		QString nText2 = model->item(i + 1, 1)->text();

		model->item(i, 0)->setText(nText);
		model->item(i, 1)->setText(nText2);

		model->item(i + 1, 0)->setText(text);
		model->item(i + 1, 1)->setText(text2);

		ui->treeView->setCurrentIndex(model->index(i + 1, 0));
	}
}
