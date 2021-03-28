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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "diceparser.h"

#include <QTcpSocket>

struct ConnectionInfo {
  QString m_host;
  QString m_channel;
  QString m_nickname;
  int m_port;
};

class BotIrcDiceParser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ConnectionInfo info READ info WRITE setInfo NOTIFY infoChanged)

public:
    explicit BotIrcDiceParser(QObject* parent= 0);
    virtual ~BotIrcDiceParser();

    QString diceToText(QList<ExportedDiceResult>& dice, bool highlight, bool homogeneous);
    QString startDiceParsing(QString& cmd, bool highlight);
    ConnectionInfo info() const;

public slots:
    void errorOccurs(QAbstractSocket::SocketError);
    void connectToServer();
    void setInfo(const ConnectionInfo & info);


private slots:
    void readData();
    void disconnectFromServer();
    void authentificationProcess();
    void joinChannel();
    void setRegisterName();


signals:
    void infoChanged();

private:
    QTcpSocket* m_socket;
    DiceParser* m_parser;
    ConnectionInfo m_info;
};

#endif // MAINWINDOW_H
