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
{


}
void Range::setValue(qint64 s,qint64 e)
{
    m_start = s;
    m_end=e;
}

qint64 Range::hasValid(Die* m,bool recursive) const
{
    if(recursive)
    {
        qint64 i = 0;
        foreach(qint64 value, m->getListValue())
        {
            if((value>=m_start)&&(value<=m_end))
            {
                ++i;
            }
        }
        return i;
    }
    else if((m->getLastRolledValue()>=m_start)&&(m->getLastRolledValue()<=m_end))
    {
        return 1;
    }
    return 0;
}
QString Range::toString()
{
    return QString("Range_%1_%2").arg(m_start).arg(m_end);
}
quint8 Range::getValidRangeSize(quint64 faces) const
{
    Q_UNUSED(faces);
    return m_end-m_start;
}
