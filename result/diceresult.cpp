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

#include "diceresult.h"
#include <QDebug>

DiceResult::DiceResult()
    : m_operator(Die::PLUS)
{
    m_resultTypes= (DICE_LIST | SCALAR);
    m_homogeneous = true;
}
void DiceResult::insertResult(Die* die)
{
    m_diceValues.append(die);
}
QList<Die*>& DiceResult::getResultList()
{
    return m_diceValues;
}
bool DiceResult::isHomogeneous() const
{
    return m_homogeneous;
}
void DiceResult::setHomogeneous(bool b)
{
    m_homogeneous = b;
}

void DiceResult::setResultList(QList<Die*> list)
{
	qDeleteAll(m_diceValues.begin(), m_diceValues.end());
    m_diceValues.clear();
    m_diceValues << list;
}
DiceResult::~DiceResult()
{
    if(!m_diceValues.isEmpty())
    {
        qDeleteAll(m_diceValues.begin(), m_diceValues.end());
        m_diceValues.clear();
    }
}
QVariant DiceResult::getResult(RESULT_TYPE type)
{
    switch (type)
    {
        case SCALAR:
        {
             return getScalarResult();
        }
        case DICE_LIST:
        {
            return QVariant();
        }
        default:
            break;
    }
    return QVariant();

}
bool DiceResult::contains(Die* die, const std::function<bool(const Die*,const Die*)> equal)
{
    for(auto value : m_diceValues)
    {
        if(equal(value,die))
        {
            return true;
        }
    }
    return false;
}
qreal DiceResult::getScalarResult()
{
    if(m_diceValues.size()==1)
    {
        return m_diceValues[0]->getValue();
    }
    else
    {
        qint64 scalar=0;
        int i = 0;
        for(auto tmp : m_diceValues)
        {
            if(i>0)
            {
                switch(m_operator)
                {
                case Die::PLUS:
                    scalar+=tmp->getValue();
                    break;
                case Die::MULTIPLICATION:
                    scalar*=tmp->getValue();
                    break;
                case Die::MINUS:
                    scalar-=tmp->getValue();
                    break;
                case Die::DIVIDE:
                    if(tmp->getValue()!=0)
                    {
                        scalar/=tmp->getValue();
                    }
                    else
                    {
                        /// @todo Error cant divide by 0. Must be displayed.
                    }
                    break;
                }
            }
            else
            {
               scalar=tmp->getValue();
            }
            ++i;
        }
        return scalar;
    }

    return 0;
}

Die::ArithmeticOperator DiceResult::getOperator() const
{
    return m_operator;
}

void DiceResult::clear()
{
    m_diceValues.clear();
}

void DiceResult::setOperator(const Die::ArithmeticOperator& dieOperator)
{
    m_operator = dieOperator;
}
QString DiceResult::toString(bool wl)
{
    QStringList scalarSum;
    for(auto die : m_diceValues)
    {
        scalarSum << QString::number(die->getValue());
    }
    if(wl)
    {
		return QStringLiteral("%3 [label=\"DiceResult Value %1 dice %2\"]").arg(getScalarResult()).arg(scalarSum.join('_')).arg(m_id);
	}
	else
	{
		return m_id;
	}
}
Result* DiceResult::getCopy() const
{
    auto copy = new DiceResult();
    copy->setHomogeneous(m_homogeneous);
    copy->setOperator(m_operator);
    QList<Die*> list;
    for(auto die : m_diceValues)
    {
        auto newdie = new Die(*die);
        die->displayed();
        list.append(newdie);
    }
    copy->setResultList(list);
    return copy;
}
