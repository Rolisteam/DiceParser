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

#include "die.h"

#include <QDateTime>
#include <QDebug>
#include <QColor>

Die::Die()
    : m_hasValue(false),m_displayStatus(false),m_highlighted(true),m_base(1),m_color("")
{
    uint seed = quintptr(this) + QDateTime::currentDateTime().toMSecsSinceEpoch();
    qsrand(seed);


}
Die::Die(const Die& die)
{
    m_value = die.m_value;
    m_rollResult = die.m_rollResult;
    m_selected = die.m_selected;
    m_hasValue = die.m_hasValue;
    m_displayStatus = die.m_displayStatus;
    m_faces = die.m_faces;
    m_highlighted = die.m_highlighted;
    m_base = die.m_base;
}

void Die::setValue(qint64 r)
{
    m_value = r;
    m_hasValue = true;
}

void Die::insertRollValue(qint64 r)
{
    m_rollResult.append(r);
}

void Die::setSelected(bool b)
{
    m_selected = b;
}


bool Die::isSelected() const
{
    return m_selected;
}
qint64 Die::getValue() const
{
    if(m_hasValue)
        return m_value;
    else
    {
        qint64 value=0;
        foreach(qint64 tmp,m_rollResult)
        {
            value+=tmp;
        }
        return value;
    }
}
QList<qint64> Die::getListValue() const
{
    return m_rollResult;
}
bool Die::hasChildrenValue()
{
    return m_rollResult.size()>1?true:false;
}
void Die::replaceLastValue(qint64 value)
{
    m_rollResult.removeLast();
   insertRollValue(value);
}

void Die::roll(bool adding)
{
    if(m_faces!=0)
    {
        quint64 value=(qrand()%m_faces)+m_base;
        if((adding)||(m_rollResult.isEmpty()))
        {
            insertRollValue(value);
        }
        else
        {
            replaceLastValue(value);
        }
    }
}

quint64 Die::getFaces() const
{
    return m_faces;
}

void Die::setFaces(quint64 face)
{
    m_faces=face;
}
qint64 Die::getLastRolledValue()
{
    if(!m_rollResult.isEmpty())
    {
        return m_rollResult.last();
    }
    else
        return 0;
}
bool Die::hasBeenDisplayed() const
{
    return m_displayStatus;
}
void Die::displayed()
{
    m_displayStatus = true;
}
void Die::setHighlighted(bool a)
{
    m_highlighted = a;
}

bool Die::isHighlighted() const
{
    return m_highlighted;
}
void Die::setBase(qint64 base)
{
        m_base = base;
}
QString Die::getColor() const
{
    return m_color;
}

void Die::setColor(const QString &color)
{
    m_color = color;
}

