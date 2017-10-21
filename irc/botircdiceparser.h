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

class BotIrcDiceParser : public QObject
{
    Q_OBJECT

public:
    explicit BotIrcDiceParser(QObject *parent = 0);
    virtual ~BotIrcDiceParser();

    QString diceToText(QList<ExportedDiceResult>& dice, bool highlight, bool homogeneous);
    QString startDiceParsing(QString &cmd, bool highlight);
public slots:
    void errorOccurs(QAbstractSocket::SocketError);
    void connectToServer();
private:
    //Ui::BotIrcDiceParser *ui;
    QTcpSocket * m_socket;
    DiceParser* m_parser;

private slots:
     void readData();
     void disconnectFromServer();
     void authentificationProcess();
     void joinChannel();
     void setRegisterName();
};

#endif // MAINWINDOW_H
