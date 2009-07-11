#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include <iostream>
#include <string>

using namespace std;

string hostname, username, password;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnConnect_clicked()
{
    ConnectDialog d;
    d.show();
    d.raise();
    d.activateWindow();
    if (d.exec())
    {
        hostname = d.getLineEditText().toStdString();
        username = d.getLineEdit2Text().toStdString();
        password = d.getLineEdit3Text().toStdString();
    }
    cout << hostname << "\n";
    cout << username << "\n";
    cout << password << "\n";
}
