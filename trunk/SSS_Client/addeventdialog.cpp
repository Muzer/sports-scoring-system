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
	model->setColumnCount(2);
	model->setHorizontalHeaderLabels(QStringList() << "Name" << "Type");

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

QString AddEventDialog::getYeargroup()
{
	return ui->comboBox->currentText();
}

QString AddEventDialog::getEventName()
{
	return ui->lineEdit->text();
}

QString AddEventDialog::getModelText(int row, int column)
{
	return model->item(row, column)->text();
}

int AddEventDialog::rows()
{
	return model->rowCount();
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
	model->appendRow(QList<QStandardItem *>() << new QStandardItem("Section") << new QStandardItem("TEXT"));
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

void AddEventDialog::ok()
{
	bool error = false;
	if (model->rowCount() <= 0)
	{
		QMessageBox::critical(this, "No Sections", "There are currently no sections.");
		error = true;
	}

	if (!error)
	{
		if (ui->lineEdit->text().isEmpty())
		{
			QMessageBox::critical(this, "No Event Name", "The Event Name is empty.");
			error = true;
		}
	}

	for (int i = 0; i < model->rowCount(); i++)
	{
		if (!error)
		{
			if (model->item(i, 1)->text() != "TEXT" && model->item(i, 1)->text() != "TIME")
			{
				stringstream ss;
				ss << "The Type on row " <<( i + 1) << " is incorrent. It should be either TEXT or TIME.";
				QMessageBox::critical(this, "Incorrect Type", ss.str().c_str());
				error = true;
			}
		}
	}

	if (!error)
		emit accept();
}
