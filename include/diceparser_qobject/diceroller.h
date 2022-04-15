/***************************************************************************
 *	Copyright (C) 2017 by Renaud Guezennec                             *
 *   https://rolisteam.org/contact                   *
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

#include <QObject>
#include <diceparser/diceparser.h>

#include <diceparser_qobject/diceparser_qobject_global.h>

class DICEPARSER_QOBJECT_EXPORT DiceRoller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal result READ result NOTIFY resultChanged)
    Q_PROPERTY(QString dicelist READ diceList NOTIFY diceListChanged)
    Q_PROPERTY(QString resultStr READ resultStr NOTIFY resultStrChanged)
    Q_PROPERTY(QString command READ command WRITE setCommand NOTIFY commandChanged)
    Q_PROPERTY(QString error READ error WRITE setError NOTIFY errorOccurs)

public:
    DiceRoller(QObject* parent= nullptr);

    QString diceList() const;
    QString resultStr() const;
    QString command() const;
    qreal result() const;
    QString error() const;
    QList<DiceAlias*>* aliases() const;

    DiceParser* parser();

signals:
    void resultChanged();
    void diceListChanged();
    void resultStrChanged();
    void commandChanged();
    void errorOccurs();

public slots:
    void start();
    void readErrorAndWarning();
    void setCommand(const QString& cmd);
    void setError(const QString& error);

    /*protected:
        QString diceToText(QList<ExportedDiceResult>& diceList);*/

private:
    DiceParser m_diceparser;
    qreal m_result;
    QString m_diceList;
    QString m_resultStr;
    QString m_command;
    QString m_error;
};

#endif // DICEROLLER_H
