/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://www.rolisteam.org/contact                      *
*                                                                          *
*  This file is part of DiceParser                                         *
*                                                                          *
* DiceParser is free software; you can redistribute it and/or modify       *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation; either version 2 of the License, or        *
* (at your option) any later version.                                      *
*                                                                          *
* This program is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with this program; if not, write to the                            *
* Free Software Foundation, Inc.,                                          *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 *
***************************************************************************/
#include "botircdiceparser.h"

#include <math.h>
#include <QDebug>
#include <QString>

BotIrcDiceParser::BotIrcDiceParser(QObject *parent) :
    QObject(parent)
{

    // Create socket
    m_socket = new QTcpSocket(this);

    m_parser = new DiceParser();

    // Connect signals and slots!
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(m_socket,SIGNAL(connected()),this,SLOT(authentificationProcess()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(connectToServer()));
    connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorOccurs(QAbstractSocket::SocketError)));

}

BotIrcDiceParser::~BotIrcDiceParser()
{
  //  delete ui;
}
void BotIrcDiceParser::connectToServer()
{
    qDebug() << "start connection";
    m_socket->connectToHost(QString("irc.freenode.net"), 8001);
}
void BotIrcDiceParser::errorOccurs(QAbstractSocket::SocketError)
{
       qDebug() << "ERROR" << m_socket->errorString();
}

void BotIrcDiceParser::readData()
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
                if(!cmd.isEmpty())
                {

                    cmd = cmd.simplified();
                    QString result = startDiceParsing(cmd,true);
                    if(!result.isEmpty())
                    {
                        QString msg("PRIVMSG #RolisteamOfficial :%1 \r\n");
                        m_socket->write(msg.arg(result).toLatin1());
                    }
                }

            }
            else
            {
                return;
            }
//

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
    if(readLine.contains(QLatin1String("Found your hostname")))
    {
        authentificationProcess();
    }
    if(readLine.contains(QLatin1String("msg NickServ identify")))
    {
        setRegisterName();
    }
    // Add to ouput
    //ui->m_output->append(readLine.trimmed());
    QMessageLogger().debug() << readLine.trimmed();
    // Next data??
    if(m_socket->canReadLine()) readData();
}
void BotIrcDiceParser::setRegisterName()
{
    m_socket->write(QLatin1String("msg NickServ identify  \r\n").data());
    joinChannel();
}

void BotIrcDiceParser::disconnectFromServer()
{
    // Disconnect from IRC server
    m_socket->write("QUIT Good bye \r\n"); // Good bye is optional message
    m_socket->flush();
    m_socket->disconnect(); // Now we can try it :-)

}
 void BotIrcDiceParser::authentificationProcess()
 {
     qDebug() << "authentification";
     m_socket->write(QLatin1String("NICK rolisteamDice \r\n").data());
     m_socket->write(QLatin1String("USER rolisteamDice rolisteamDice rolisteamDice :rolisteamDice BOT \r\n").data());


 }
void BotIrcDiceParser::joinChannel()
{
    m_socket->write(QLatin1String("JOIN #RolisteamOfficial \r\n").data());
}
QString BotIrcDiceParser::diceToText(ExportedDiceResult& dice,bool highlight,bool homogeneous)
{
    QStringList resultGlobal;
        foreach(int face, dice.keys())
        {
               QStringList result;
               ListDiceResult diceResult =  dice.value(face);
               //patternColor = patternColorarg();
               foreach (HighLightDice tmp, diceResult)
               {
                    QStringList diceListStr;
                    QStringList diceListChildren;


                    for(int i =0; i < tmp.getResult().size(); ++i)
                    {
                        qint64 dievalue = tmp.getResult()[i];
                        QString prefix("%1");

                        if((tmp.isHighlighted())&&(highlight))
                        {
                            if(tmp.getColor().isEmpty()|| tmp.getColor()=="black")
                            {
                                prefix = "%1";
                            }
                            if(tmp.getColor()=="white")
                            {
                                prefix = "%1";
                            }
                            if(tmp.getColor()=="blue")
                            {
                                prefix = "%1";
                            }
                            if(tmp.getColor()=="red")
                            {
                                prefix = "%1";
                            }
                        }

                        if(i==0)
                        {
                            diceListStr << prefix.arg(QString::number(dievalue));
                        }
                        else
                        {
                            diceListChildren << prefix.arg(QString::number(dievalue));
                        }
                    }
                    if(!diceListChildren.isEmpty())
                    {
                        diceListStr << QString("[%1]").arg(diceListChildren.join(' '));
                    }

                    result << diceListStr.join(' ');
                   // qDebug() << result << tmp.first << tmp.second;
               }

               if(dice.keys().size()>1)
               {
                  resultGlobal << QString(" d%2:(%1)").arg(result.join(',')).arg(face);
               }
               else
               {
                   resultGlobal << result;
               }
        }
    return resultGlobal.join(' ');
}

QString BotIrcDiceParser::startDiceParsing(QString& cmd,bool highlight)
{
    QString result;
    QTextStream out(&result);
    if(m_parser->parseLine(cmd))
    {

            m_parser->Start();
            if(!m_parser->getErrorMap().isEmpty())
            {
                out << "Error" << m_parser->humanReadableError()<< "\n";
                return QString();
            }

            ExportedDiceResult list;
            bool homogeneous = true;
            m_parser->getLastDiceResult(list,homogeneous);
            QString diceText = diceToText(list,highlight,homogeneous);
            QString scalarText;
            QString str;

            if(m_parser->hasIntegerResultNotInFirst())
            {
                scalarText = QString("%1").arg(m_parser->getLastIntegerResult());
            }
            else if(!list.isEmpty())
            {
                scalarText = QString("%1").arg(m_parser->getSumOfDiceResult());
            }
            if(highlight)
                str = QString("Result: %1, details:[%3 (%2)]").arg(scalarText).arg(diceText).arg(m_parser->getDiceCommand());
            else
                str = QString("Result: %1, details:[%3 (%2)]").arg(scalarText).arg(diceText).arg(m_parser->getDiceCommand());

            if(m_parser->hasStringResult())
            {
                str = m_parser->getStringResult();
            }
            out << str << "\n";

    }
    else
    {
        out << m_parser->humanReadableError()<< "\n";;
    }

    return result;
}
