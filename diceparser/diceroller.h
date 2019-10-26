/***************************************************************************
 *	Copyright (C) 2017 by Renaud Guezennec                             *
 *   http://www.rolisteam.org/contact                   *
 *                                                                         *
 *   Rolisteam is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef DICEROLLER_H
#define DICEROLLER_H

#include "diceparser.h"
#include <QObject>

class DiceRoller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal result READ getResult NOTIFY resultChanged)
    Q_PROPERTY(QString dicelist READ getDiceList NOTIFY diceListChanged)
    Q_PROPERTY(QString resultStr READ getResultStr NOTIFY resultStrChanged)
    Q_PROPERTY(QString command READ getCommand WRITE setCommand NOTIFY commandChanged)
    Q_PROPERTY(QString error READ getError WRITE setError NOTIFY errorOccurs)

public:
    DiceRoller();

    QString getDiceList() const;
    QString getResultStr() const;
    QString getCommand() const;
    qreal getResult() const;
    void setCommand(const QString& cmd);

    QString getError() const;
    void setError(const QString& error);

signals:
    void resultChanged();
    void diceListChanged();
    void resultStrChanged();
    void commandChanged();
    void errorOccurs();

public slots:
    void start();

protected:
    QString diceToText(QList<ExportedDiceResult>& diceList);

private:
    DiceParser m_diceparser;
    qreal m_result;
    QString m_diceList;
    QString m_resultStr;
    QString m_command;
    QString m_error;
};

#endif // DICEROLLER_H
