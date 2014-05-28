/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://renaudguezennec.homelinux.org/accueil,3.html                      *
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
#include <QDateTime>

#include "die.h"

#include <QDebug>

Die::Die()
    : m_hasValue(false),m_displayStatus(false)
{
    uint seed = quintptr(this) + QDateTime::currentDateTime().toMSecsSinceEpoch();
    qsrand(seed);
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
    quint64 value=(qrand()%m_faces)+1;

    if((adding)||(m_rollResult.isEmpty()))
    {
        insertRollValue(value);
    }
    else
    {
        replaceLastValue(value);
    }
}

quint64 Die::getFaces()
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
bool Die::hasBeenDisplayed()
{
    return m_displayStatus;
}
void Die::displayed()
{
    m_displayStatus = true;
}
