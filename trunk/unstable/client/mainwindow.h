#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>

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

private:
    Ui::MainWindow *ui;

private slots:
        void btnConnect_clicked();
};

#endif // MAINWINDOW_H
