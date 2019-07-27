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
#include "range.h"

Range::Range() : m_hasEnd(false), m_hasStart(false), m_emptyRange(false) {}
void Range::setValue(qint64 s, qint64 e)
{
    m_start= s;
    m_end= e;

    m_hasEnd= true;
    m_hasStart= true;
}

qint64 Range::hasValid(Die* m, bool recursive, bool unhighlight) const
{
    qint64 result= 0;
    if(recursive)
    {
        for(qint64& value : m->getListValue())
        {
            if((value >= m_start) && (value <= m_end))
            {
                ++result;
            }
        }
    }
    else if((m->getLastRolledValue() >= m_start) && (m->getLastRolledValue() <= m_end))
    {
        ++result;
    }
    if((unhighlight) && (result == 0))
    {
        m->setHighlighted(false);
    }
    return result;
}
QString Range::toString()
{
    return QStringLiteral("[%1-%2]").arg(m_start).arg(m_end);
}
Dice::CONDITION_STATE Range::isValidRangeSize(const std::pair<qint64, qint64>& range) const
{
    auto minRange= std::min(m_start, m_end);
    auto minPossibleValue= std::min(range.first, range.second);

    auto maxRange= std::max(m_start, m_end);
    auto maxPossibleValue= std::max(range.first, range.second);

    if(minRange == minPossibleValue && maxRange == maxPossibleValue)
        return Dice::CONDITION_STATE::ALWAYSTRUE;
    else if(maxRange < minPossibleValue || minRange > maxPossibleValue)
        return Dice::CONDITION_STATE::UNREACHABLE;
    else
        return Dice::CONDITION_STATE::UNREACHABLE;
}
void Range::setStart(qint64 start)
{
    m_start= start;
    m_hasStart= true;
}
void Range::setEnd(qint64 end)
{
    m_end= end;
    m_hasEnd= true;
}

bool Range::isFullyDefined() const
{
    return (m_hasEnd && m_hasStart);
}
qint64 Range::getStart() const
{
    return m_start;
}
qint64 Range::getEnd() const
{
    return m_end;
}
void Range::setEmptyRange(bool b)
{
    m_emptyRange= b;
}

bool Range::isEmptyRange()
{
    return m_emptyRange;
}
Validator* Range::getCopy() const
{
    Range* val= new Range();
    val->setEmptyRange(m_emptyRange);
    if(m_hasEnd)
    {
        val->setEnd(m_end);
    }
    if(m_hasStart)
    {
        val->setStart(m_start);
    }
    return val;
}
