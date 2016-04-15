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
    m_socket = new QTcpSocket(this);

    m_parser = new DiceParser();

    // Connect signals and slots!
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(m_socket,SIGNAL(connected()),this,SLOT(authentificationProcess()));
    connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorOccurs(QAbstractSocket::SocketError)));
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
    qDebug() << "start connection";
    m_socket->connectToHost(QString("irc.freenode.net"), 8001);
}
void MainWindow::errorOccurs(QAbstractSocket::SocketError)
{
       qDebug() << "ERROR" << m_socket->errorString();
}

void MainWindow::readData()
{

    qDebug() << "Reply";
    QString readLine = m_socket->readLine();

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
                    QString msg("PRIVMSG #Rolisteam :%1 \r\n");
                    m_socket->write(msg.arg(result).toLatin1());
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

            m_socket->write(resp.toLatin1());
        }
    }
    if(readLine.contains("Found your hostname"))
    {
        authentificationProcess();
    }
    // Add to ouput
    ui->m_output->append(readLine.trimmed());
    // Next data??
    if(m_socket->canReadLine()) readData();
}

void MainWindow::disconnectFromServer()
{
    // Disconnect from IRC server
    m_socket->write("QUIT Good bye \r\n"); // Good bye is optional message
    m_socket->flush();
    m_socket->disconnect(); // Now we can try it :-)
}
 void MainWindow::authentificationProcess()
 {
     qDebug() << "authentification";
     m_socket->write("NICK rolisteamDice \r\n");
     m_socket->write("USER rolisteamDice rolisteamDice rolisteamDice :rolisteamDice BOT \r\n");
     m_socket->write("MSG NickServ identify  \r\n");

 }
void MainWindow::joinChannel()
{
    m_socket->write("JOIN #Rolisteam \r\n");
}
