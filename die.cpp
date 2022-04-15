/***************************************************************************
 * Copyright (C) 2014 by Renaud Guezennec                                   *
 * https://rolisteam.org/contact                      *
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
#include <QUuid>
#include <algorithm>
#include <array>
#include <chrono>

void Die::buildSeed()
{
    static bool init= false;
    if(init)
        return;
    std::array<int, std::mt19937::state_size> seed_data;
    std::random_device r;
    std::generate_n(seed_data.data(), seed_data.size(), std::ref(r));
    std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
    s_rng= std::mt19937(seq);
    init= true;
}

std::mt19937 Die::s_rng;

Die::Die()
    : m_uuid(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_hasValue(false)
    , m_displayStatus(false)
    , m_highlighted(true)
    , m_base(1)
    , m_color("")
    , m_op(Dice::ArithmeticOperator::PLUS) //,m_mt(m_randomDevice)
{
    buildSeed();
}

Die::Die(const Die& die)
{
    m_value= die.m_value;
    m_rollResult= die.m_rollResult;
    m_selected= die.m_selected;
    m_hasValue= die.m_hasValue;
    m_uuid= die.m_uuid;
    m_displayStatus= die.m_displayStatus;
    m_maxValue= die.m_maxValue;
    m_highlighted= die.m_highlighted;
    m_base= die.m_base;
    m_color= die.getColor();
    m_op= die.getOp();
    // auto seed= std::chrono::high_resolution_clock::now().time_since_epoch().count();
    // m_rng= std::mt19937(quintptr(this) + static_cast<unsigned long long>(seed));
}

void Die::setValue(qint64 r)
{
    m_value= r;
    m_hasValue= true;
}

void Die::insertRollValue(qint64 r)
{
    m_rollResult.append(r);
}

void Die::setSelected(bool b)
{
    m_selected= b;
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
        qint64 value= 0;
        int i= 0;
        for(qint64 tmp : m_rollResult)
        {
            if(i > 0)
            {
                switch(m_op)
                {
                case Dice::ArithmeticOperator::PLUS:
                    value+= tmp;
                    break;
                case Dice::ArithmeticOperator::MULTIPLICATION:
                    value*= tmp;
                    break;
                case Dice::ArithmeticOperator::MINUS:
                    value-= tmp;
                    break;
                case Dice::ArithmeticOperator::INTEGER_DIVIDE:
                case Dice::ArithmeticOperator::DIVIDE:
                    if(tmp != 0)
                    {
                        value/= tmp;
                    }
                    else
                    {
                        // error();
                    }
                    break;
                case Dice::ArithmeticOperator::POW:
                    value= static_cast<qint64>(std::pow(value, tmp));
                    break;
                }
            }
            else
            {
                value= tmp;
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
    return m_rollResult.size() > 1 ? true : false;
}
void Die::replaceLastValue(qint64 value)
{
    m_rollResult.removeLast();
    insertRollValue(value);
}

void Die::roll(bool adding)
{
    if(m_maxValue != 0)
    {
        // quint64 value=(qrand()%m_faces)+m_base;
        std::uniform_int_distribution<qint64> dist(m_base, m_maxValue);
        qint64 value= dist(s_rng);
        if((adding) || (m_rollResult.isEmpty()))
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
    return std::abs(m_maxValue - m_base) + 1;
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
    setDisplayed(true);
}
void Die::setDisplayed(bool b)
{
    m_displayStatus= b;
}
void Die::setHighlighted(bool a)
{
    m_highlighted= a;
}

bool Die::isHighlighted() const
{
    return m_highlighted;
}
void Die::setBase(qint64 base)
{
    m_base= base;
}
qint64 Die::getBase()
{
    return m_base;
}
QString Die::getColor() const
{
    return m_color;
}

void Die::setColor(const QString& color)
{
    m_color= color;
}

qint64 Die::getMaxValue() const
{
    return m_maxValue;
}

void Die::setMaxValue(const qint64& maxValue)
{
    m_maxValue= maxValue;
}

Dice::ArithmeticOperator Die::getOp() const
{
    return m_op;
}

void Die::setOp(const Dice::ArithmeticOperator& op)
{
    m_op= op;
}
QString Die::getUuid() const
{
    return m_uuid;
}

void Die::setUuid(const QString& uuid)
{
    m_uuid= uuid;
}
