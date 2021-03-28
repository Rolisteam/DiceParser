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

#include <QDebug>
#include <QString>
#include <math.h>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QString colorToIrcCode(QString str)
{
  if(str == QStringLiteral("reset"))
  {
    return {};
  }
  return str;
}

BotIrcDiceParser::BotIrcDiceParser(QObject* parent) : QObject(parent)
{
    // Create socket
    m_socket= new QTcpSocket(this);

    m_parser= new DiceParser();

    // Connect signals and slots!
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readData()));
    //connect(m_socket, SIGNAL(connected()), this, SLOT(authentificationProcess()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(connectToServer()));
    connect(
        m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorOccurs(QAbstractSocket::SocketError)));
}

BotIrcDiceParser::~BotIrcDiceParser()
{
    //  delete ui;
}
void BotIrcDiceParser::connectToServer()
{
    qDebug() << "start connection";
    m_socket->connectToHost(m_info.m_host, m_info.m_port);
}

void BotIrcDiceParser::setInfo(const ConnectionInfo &info)
{
  if(info.m_host == m_info.m_host && info.m_channel == m_info.m_channel)
    return;
  m_info = info;
  emit infoChanged();
}
void BotIrcDiceParser::errorOccurs(QAbstractSocket::SocketError)
{
    qDebug() << "ERROR" << m_socket->errorString();
}

void BotIrcDiceParser::readData()
{
    QString readLine= m_socket->readLine();

    if(readLine.startsWith("!"))
        readLine= readLine.remove(0, 1);

    if(readLine.contains("!"))
    {
        // qDebug()<< "in /dice";
        QString dice= ".*PRIVMSG.*!(.*)";
        QRegExp exp(dice);
        exp.indexIn(readLine);

        QStringList list= exp.capturedTexts();
        qDebug() << list;
        if(list.size() == 2)
        {
            QString cmd= list[1];
            if(!cmd.isEmpty())
            {
                cmd= cmd.simplified();
                QString result= startDiceParsing(cmd, true);
                if(!result.isEmpty())
                {
                    QString msg("PRIVMSG %2 :%1 \r\n");
                    m_socket->write(msg.arg(result, m_info.m_channel).toLatin1());
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
        QString dice= "PING :(.*)";
        QRegExp exp(dice);
        exp.indexIn(readLine);
        QStringList list= exp.capturedTexts();
        if(list.size() == 2)
        {
            QString resp= "PONG :" + list[1];

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
    // ui->m_output->append(readLine.trimmed());
    QMessageLogger().debug() << readLine.trimmed();
    // Next data??
    if(m_socket->canReadLine())
        readData();
}
void BotIrcDiceParser::setRegisterName()
{
    qDebug() << "IRC BOT register name";
    m_socket->write(QLatin1String("msg NickServ identify  \r\n").data());
    joinChannel();
}

void BotIrcDiceParser::disconnectFromServer()
{
    qDebug() << "IRC BOT disconnect";
    m_socket->write("QUIT Good bye \r\n"); // Good bye is optional message
    m_socket->flush();
    m_socket->disconnect(); // Now we can try it :-)
}
void BotIrcDiceParser::authentificationProcess()
{
    qDebug() << "IRC BOT authentification";
    auto nick = QStringLiteral("NICK %1 \r\n").arg(m_info.m_nickname);
    auto user = QStringLiteral("USER %1 %1 %1 :%1 BOT \r\n").arg(m_info.m_nickname);
    m_socket->write(nick.toLocal8Bit().data());
    m_socket->write(user.toLocal8Bit().data());
    joinChannel();
}
void BotIrcDiceParser::joinChannel()
{
  qDebug() << "IRC BOT Join channel";
  auto text = QStringLiteral("JOIN %1 \r\n").arg(m_info.m_channel);
  m_socket->write(text.toLocal8Bit().data());
}

QString displayCommandResult(QString json, bool withColor)
{
  QString result;
  QTextStream out(&result);
  QJsonDocument doc= QJsonDocument::fromJson(json.toUtf8());
  auto obj= doc.object();
  auto error= obj["error"].toString();
  auto warning= obj["warning"].toString();
  auto comment= obj["comment"].toString();
  auto arrayInst= obj["instructions"].toArray();
  QStringList diceResults;
  for(const auto &inst : qAsConst(arrayInst))
  {
    auto obj= inst.toObject();
    auto diceVals= obj["diceval"].toArray();
    for(const auto & diceval : qAsConst(diceVals))
    {
      auto objval= diceval.toObject();
      auto resultStr= objval["string"].toString();
      diceResults << resultStr;
    }
  }
  auto diceList= diceResults.join(",");
  auto scalarText= obj["scalar"].toString();
  auto cmd= obj["command"].toString();
  auto resultStr= obj["string"].toString();

  if(!error.isEmpty())
  {
    out << "Error" << error << "\n";
    out.flush();
    return result;
  }

  if(!warning.isEmpty())
    out << "Warning: " << warning << "\n";

  QString str;

  if(withColor)
    str= QString("Result: %1 - details:[%3 (%2)]").arg(scalarText, diceList, cmd);
  else
    str= QString("Result: %1 - details:[%3 (%2)]").arg(scalarText, diceList, cmd);

  if(!resultStr.isEmpty() && resultStr != scalarText)
  {
    resultStr.replace("%2", diceList.trimmed());
    str= resultStr;
  }

  if(!comment.isEmpty())
  {
      out << comment << " ";
  }
  out << str << "\n";

  out.flush();
  return result;
}



/*QString BotIrcDiceParser::diceToText(QList<ExportedDiceResult>& diceList, bool highlight, bool homogeneous)
{
    QStringList global;
    for(auto dice : diceList)
    {
        QStringList resultGlobal;
        for(int face : dice.keys())
        {
            QStringList result;
            ListDiceResult diceResult= dice.value(face);
            // patternColor = patternColorarg();
            for(HighLightDice tmp: qAsConst(diceResult))
            {
                QStringList diceListStr;
                QStringList diceListChildren;

                for(int i= 0; i < tmp.getResult().size(); ++i)
                {
                    qint64 dievalue= tmp.getResult()[i];
                    QString prefix("%1");

                    if((tmp.isHighlighted()) && (highlight))
                    {
                        if(tmp.getColor().isEmpty() || tmp.getColor() == "black")
                        {
                            prefix= "%1";
                        }
                        if(tmp.getColor() == "white")
                        {
                            prefix= "%1";
                        }
                        if(tmp.getColor() == "blue")
                        {
                            prefix= "%1";
                        }
                        if(tmp.getColor() == "red")
                        {
                            prefix= "%1";
                        }
                    }

                    if(i == 0)
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

            if(dice.keys().size() > 1)
            {
                resultGlobal << QString(" d%2:(%1)").arg(result.join(',')).arg(face);
            }
            else
            {
                resultGlobal << result;
            }
        }
        global << resultGlobal.join(' ');
    }
    return global.join(' ');
}*/

QString BotIrcDiceParser::startDiceParsing(QString& cmd, bool highlight)
{
    QString result;
    QTextStream out(&result);
    if(m_parser->parseLine(cmd))
    {
        m_parser->start();

        if(!m_parser->humanReadableError().isEmpty())
        {
            out << "Error" << m_parser->humanReadableError() << "\n";
            return QString();
        }
        auto allSameColor= true;
        QString colorP;
        auto json= m_parser->resultAsJSon(
          [&colorP, &allSameColor](const QString& result, const QString& color, bool hightlight) {
            auto trueColor= color;
            if(color.isEmpty())
              trueColor= "red";

            if(colorP.isEmpty())
              colorP= trueColor;
            else if(colorP != trueColor)
              allSameColor= false;

            auto front= colorToIrcCode(trueColor);
            auto end= front.isEmpty() ? "" : colorToIrcCode("reset");
            return hightlight ? QString("%1%2%3").arg(front).arg(result).arg(end) : result;
          });

        out << displayCommandResult(json, allSameColor);
    }
    else
    {
        out << m_parser->humanReadableError() << "\n";
    }

    return result;
}

ConnectionInfo BotIrcDiceParser::info() const
{
  return m_info;
}
