/***************************************************************************
 *   Copyright (C) 2016 by Renaud Guezennec                                *
 *   http:://www.rolisteam.org/contact                                     *
 *                                                                         *
 *   rolisteam is free software; you can redistribute it and/or modify     *
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
#ifndef HighLightDice_H
#define HighLightDice_H

#include <QList>
#include <QMap>
#include <QString>

class HighLightDice
{
public:
    HighLightDice(QList<qint64> result, bool isHighlighted, QString color, bool displayed, quint64 faces,
                  const QString& uuid);
    virtual ~HighLightDice();

    QList<qint64> result() const;
    void setResult(const QList<qint64>& result);

    bool isHighlighted() const;
    void setHighlight(bool hasHighlight);

    QString color() const;
    void setColor(const QString& color);

    bool displayed() const;
    void setDisplayed(bool displayed);

    quint64 faces() const;
    void setFaces(const quint64& faces);

    QString getResultString() const;

    QString uuid() const;
    void setUuid(const QString& uuid);

private:
    QList<qint64> m_result;
    bool m_hasHighlight= true;
    QString m_color;
    bool m_displayed= false;
    quint64 m_faces;
    QString m_uuid;
};

typedef QList<HighLightDice> ListDiceResult;
typedef QMap<quint64, QList<ListDiceResult>> ExportedDiceResult;

#endif // HighLightDice_H
