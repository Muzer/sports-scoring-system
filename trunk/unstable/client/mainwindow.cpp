#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include <iostream>
#include <string>

using namespace std;

QString hostname, username, password;
QTcpSocket socket;
quint16 port = 8643;
char socketData[65535];

bool conected = 0;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    socket.disconnectFromHost();
}

void MainWindow::btnConnect_clicked()
{
    ConnectDialog d;
    d.show();
    d.raise();
    d.activateWindow();
    if (d.exec())
    {
        hostname = d.getLineEditText();
        username = d.getLineEdit2Text();
        password = d.getLineEdit3Text();
        socket.connectToHost(hostname, port);
        cout << "1";
        socket.write(("AUTH " + username.toStdString() + ";" + password.toStdString() + "$").c_str());
        memset(socketData,0,65535);
        int bad = 0;
        while(bad == 0){
            bad = socket.readLine(socketData, 65535);
        }
        if(socketData != NULL){
            char parsed[65535];
            memset(parsed,0,65535);
            unsigned int number = 0;
            unsigned int number2 = 0;
            for(number=0;number != strlen(socketData) + 1;++number){
                if(socketData[number] != 0 && socketData[number] != '$')
                    parsed[number - number2] = socketData[number];
                else{
                    if(parsed[number - number2 - 1] == '\n')
                        parsed[number - number2 - 1] = 0;
                    // Start code for checking for each result. Little example
                    if(strncmp(parsed, "AUTHGOOD",8) == 0){
                        cout << "Authentication succesful\n";
                    }
                    // End of code for checking for each result
                    memset(parsed,0,65535);
                    number2 = number + 1;
                }

            }
        }
        cout << socketData << endl;
        socket.write("hi");
    }
}
