#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include <QTcpSocket>
#include <qmessagebox.h>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setStatusText(QString str);
    void setbtnConnectText(QString str);
    bool sendToSocket(const char* str);

private:
    Ui::MainWindow *ui;

private slots:
        void btnConnect_clicked();
        void btnAddScore_clicked();

public slots:
        void Disconnected();

};

#endif
