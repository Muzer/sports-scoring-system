#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectdialog.h"
#include "newtabledialog.h"
#include <iostream>
#include <string>

using namespace std;

QString hostname, username, password;
QTcpSocket socket;
quint16 port = 8643;
char socketData[65535];
pthread_t pingthrd,livesthrd;
int retval = 99, retval2 = 99;
bool connected = 0;

char parsed[65535];
unsigned int number = 0;
unsigned int number2 = 0;

void keepalive(){
    while(1){
        cout << "Running thread\n";
        if(connected == true){
            socket.write("KEEPALIVE$");
            sleep(60);
        }
        sleep(1);
    }
}

void MainWindow::Disconnected(){
//    QMessageBox::critical(0,"Error", "Disconnected from the server. Possible causes: The server was killed, the server crashed, the keepalive thread crashed and we timed out.",QMessageBox::Ok | QMessageBox::Default,QMessageBox::NoButton,QMessageBox::NoButton);
    ui->btnConnect->setText("Connect");
    setStatusText("Disconnected from server.");
}

bool MainWindow::sendToSocket(const char* text)
{
    if(strlen(text) < 65535)
    {
        if(socket.write(text) != -1) { return true; }
        else { return false; }
        return 1;
    }
    return 0;
}

bool MainWindow::readFromSocket()
{
    socket.readLine(socketData,65535);
    if(socketData != NULL){
        memset(parsed,0,65535);
        for(number=0;number != strlen(socketData) + 1;++number){
            if(socketData[number] != 0 && socketData[number] != '$')
                parsed[number - number2] = socketData[number];
            else
            {
                if(parsed[number - number2 - 1] == '\n')
                    parsed[number - number2 - 1] = 0;
            }

        }
    }
    return true;
}

void makesureserverlives(void *obj){
    while(1){
        if(socket.state() == QAbstractSocket::UnconnectedState && connected == true){
            cout << "Error occurred - disconnected.\n";
            socket.disconnectFromHost();
            connected = false;
            static_cast<MainWindow*>(obj)->Disconnected();
       }
    }
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow)
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
    if(connected == false){
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
            if(!socket.waitForConnected(5000)){
                QMessageBox::critical(0,"Error","Failed to connect to the server. Did you misspell it? Is their firewall on?",QMessageBox::Ok | QMessageBox::Default,QMessageBox::NoButton,QMessageBox::NoButton);
                socket.disconnectFromHost();
                setStatusText("Failed to connect to server.");
                return;
            }

            sendToSocket(("AUTH " + username.toStdString() + ";" + password.toStdString() + "$\n").c_str());

            memset(socketData,0,65535);
            if(!socket.waitForReadyRead(5000)){
                QMessageBox::critical(0,"Error","The server did not report whether or not authentication was sucessful. Did the server crash? Is the server on port 8643 the correct one?",QMessageBox::Ok | QMessageBox::Default,QMessageBox::NoButton,QMessageBox::NoButton);
                socket.disconnectFromHost();
                setStatusText("Did not recieve data from server.");
                return;
            }
            readFromSocket();
            if(strncmp(parsed, "AUTHGOOD",8) == 0){
                QMessageBox::information(0,"Success","Succesfully connected to the server and logged in! ",QMessageBox::Ok | QMessageBox::Default,QMessageBox::NoButton,QMessageBox::NoButton);
                connected = true;
                if (not retval == 0) { cout << "Starting thread." << endl; retval = pthread_create((pthread_t*)&pingthrd,NULL,(void* (*)(void*))keepalive,NULL); }
                if (not retval2 == 0) { cout << "Starting thread." << endl; retval2 = pthread_create((pthread_t*)&livesthrd,NULL,(void* (*)(void*))makesureserverlives,this); }
                sendToSocket("REQUESTDB$");
                readFromSocket();
                if(strncmp(parsed, "NOTABLE",7) == 0){
                    NewTableDialog d;
                    d.show();
                    d.activateWindow();
                    d.raise();
                    if ( d.exec() )
                    {
                        ui->btnConnect->setText("Disconnect");
                        setStatusText("Connected to server.");
                    }
                    else { socket.disconnectFromHost(); }
                }
                else if(strncmp(parsed, "COMMENCEDB",7) == 0){
                    ui->btnConnect->setText("Disconnect");
                    setStatusText("Connected to server.");
                }
                else { QMessageBox::critical(0,"Error","Failed to get table data from server.",QMessageBox::Ok | QMessageBox::Default,QMessageBox::NoButton,QMessageBox::NoButton); socket.disconnectFromHost(); }
                memset(parsed,0,65535);
                number2 = number + 1; }
            if(strncmp(parsed, "AUTHBAD",7) == 0){
                QMessageBox::critical(0,"Error","Failed to authenticate. Is your username and password correct?",QMessageBox::Ok | QMessageBox::Default,QMessageBox::NoButton,QMessageBox::NoButton);
                socket.disconnectFromHost();
            }
        }
    } else {
        socket.disconnectFromHost();
        QMessageBox::information(0,"Success","Disconnected.",QMessageBox::Ok | QMessageBox::Default,QMessageBox::NoButton,QMessageBox::NoButton);
        connected = false;
        ui->btnConnect->setText("Connect");
        setStatusText("Disconnected from server.");
    }
}

void MainWindow::btnAddScore_clicked()
{
    if (connected == true)
    {
    }
    else { QMessageBox::critical(0,"Error","You are not connected to a server.",QMessageBox::Ok | QMessageBox::Default,QMessageBox::NoButton,QMessageBox::NoButton); }
}

void MainWindow::setStatusText(QString str) { ui->lblStatus->setText(str); }

void MainWindow::setbtnConnectText(QString str) { ui->btnConnect->setText(str); }
