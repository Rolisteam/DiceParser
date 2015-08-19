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
#include "range.h"

Range::Range()
    : m_hasEnd(false),m_hasStart(false)
{


}
void Range::setValue(qint64 s,qint64 e)
{
    m_start = s;
    m_end=e;

    m_hasEnd = true;
    m_hasStart = true;
}

qint64 Range::hasValid(Die* m,bool recursive, bool unhighlight) const
{
    qint64 result = 0;
    if(recursive)
    {
        foreach(qint64 value, m->getListValue())
        {
            if((value>=m_start)&&(value<=m_end))
            {
                ++result;
            }
        }
    }
    else if((m->getLastRolledValue()>=m_start)&&(m->getLastRolledValue()<=m_end))
    {
        ++result;
    }
    if((unhighlight)&&(result==0))
    {
        m->setHighlighted(false);
    }
    return result;
}
QString Range::toString()
{
    return QString("[%1-%2]").arg(m_start).arg(m_end);
}
quint64 Range::getValidRangeSize(quint64 faces) const
{
    Q_UNUSED(faces);
    return m_end-m_start;
}
void Range::setStart(qint64 start)
{
    m_start = start;
    m_hasStart = true;
}
void Range::setEnd(qint64 end)
{
    m_end = end;
    m_hasEnd = true;
}

bool Range::isFullyDefined()
{
    return (m_hasEnd & m_hasStart);
}
qint64 Range::getStart() const
{
 return m_start;
}
qint64 Range::getEnd() const
{
    return m_end;
}
