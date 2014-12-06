#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "diceparser.h"

#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket * socket;
    DiceParser* m_parser;

private slots:
     void readData();
     void connectToServer();
     void disconnectFromServer();
     void authentificationProcess();
     void joinChannel();
};

#endif // MAINWINDOW_H
