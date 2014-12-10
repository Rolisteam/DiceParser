#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <math.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create socket
    socket = new QTcpSocket(this);

    m_parser = new DiceParser();

    // Connect signals and slots!
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket,SIGNAL(connected()),this,SLOT(authentificationProcess()));
    connect(ui->m_connectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
    connect(ui->m_disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectFromServer()));
    connect(ui->m_joinButton, SIGNAL(clicked()), this, SLOT(joinChannel()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::connectToServer()
{
    socket->connectToHost(QString("irc.epiknet.org"), 6667);
}

void MainWindow::readData()
{
    QString readLine = socket->readLine();

    if(readLine.startsWith("!"))
         readLine = readLine.remove(0,1);


    if(readLine.contains("!"))
    {

       // qDebug()<< "in /dice";
        QString dice=".*PRIVMSG.*!(.*)";
        QRegExp exp(dice);
        exp.indexIn(readLine);



            QStringList list = exp.capturedTexts();
            qDebug()<<list;
            if(list.size()==2)
            {
                QString cmd = list[1];
                if(m_parser->parseLine(cmd.simplified()))
                {
                    m_parser->Start();
                    QString result = m_parser->displayResult();
                    QString msg("PRIVMSG #opale-roliste :%1 \r\n");
                    socket->write(msg.arg(result).toLatin1());
                }
            }
            else
            {
                return;
            }


    }
    else if(readLine.contains("PING :"))
    {
        QString dice="PING :(.*)";
        QRegExp exp(dice);
        exp.indexIn(readLine);
        QStringList list = exp.capturedTexts();
        if(list.size()==2)
        {
            QString resp = "PONG :"+list[1];

            socket->write(resp.toLatin1());
        }
    }
    // Add to ouput
    ui->m_output->append(readLine.trimmed());
    // Next data??
    if(socket->canReadLine()) readData();
}

void MainWindow::disconnectFromServer()
{
    // Disconnect from IRC server
    socket->write("QUIT Good bye \r\n"); // Good bye is optional message
    socket->flush();
    socket->disconnect(); // Now we can try it :-)
}
 void MainWindow::authentificationProcess()
 {
     socket->write("NICK diceBot \r\n");
     socket->write("USER diceBot diceBot diceBot :diceBot BOT \r\n");

 }
void MainWindow::joinChannel()
{
    socket->write("JOIN #opale-roliste \r\n");
}
