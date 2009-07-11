#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"

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
    d.activateWindow();
}
