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
#include <chrono>

std::mt19937 DefaultRandomGenerator::m_rng = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count());
std::shared_ptr<RandomGenerator> Die::m_rng = std::make_shared<DefaultRandomGenerator>();

void Die::setRandomGenerator(std::shared_ptr<RandomGenerator>&& r)
{
    Die::m_rng = r;
}

Die::Die(qint64 min, qint64 max)
    : m_hasValue(false),m_displayStatus(false),m_highlighted(true),m_min(min),m_max(max),m_color(""),m_op(Die::PLUS)
{
    assert(min<=max);
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
    {
        return m_value;
    }
    else
    {
        qint64 value=0;
        int i = 0;
        for(qint64 tmp : m_rollResult)
        {
            if(i>0)
            {
                switch(m_op)
                {
                case PLUS:
                    value+=tmp;
                    break;
                case MULTIPLICATION:
                    value*=tmp;
                    break;
                case MINUS:
                    value-=tmp;
                    break;
                case DIVIDE:
                    if(tmp!=0)
                    {
                        value/=tmp;
                    }
                    else
                    {
                        //error();
                    }
                    break;
                }
            }
            else
            {
               value=tmp;
            }
            ++i;
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
    std::uniform_int_distribution<qint64> dist(m_min,m_max);
    qint64 value = dist(*m_rng);
    if((adding)||(m_rollResult.isEmpty()))
    {
        insertRollValue(value);
    }
    else
    {
        replaceLastValue(value);
    }
}

quint64 Die::getFaces() const
{
    return abs(m_max-m_min)+1;
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
QString Die::getColor() const
{
    return m_color;
}

void Die::setColor(const QString &color)
{
    m_color = color;
}

qint64 Die::getMaxValue() const
{
    return m_max;
}

Die::ArithmeticOperator Die::getOp() const
{
    return m_op;
}

void Die::setOp(const Die::ArithmeticOperator &op)
{
    m_op = op;
}
