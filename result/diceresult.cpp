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

#include "diceresult.h"
#include <QDebug>

DiceResult::DiceResult()
{
    m_resultTypes= (static_cast<int>(Dice::RESULT_TYPE::DICE_LIST) | static_cast<int>(Dice::RESULT_TYPE::SCALAR));
    m_homogeneous= true;
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
    m_homogeneous= b;
}

void DiceResult::setResultList(QList<Die*> list)
{
    m_diceValues.erase(
        std::remove_if(m_diceValues.begin(), m_diceValues.end(), [list](Die* die) { return list.contains(die); }),
        m_diceValues.end());

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
QVariant DiceResult::getResult(Dice::RESULT_TYPE type)
{
    switch(type)
    {
    case Dice::RESULT_TYPE::SCALAR:
    {
        return getScalarResult();
    }
    case Dice::RESULT_TYPE::DICE_LIST:
    {
        return QVariant::fromValue(m_diceValues);
    }
    default:
        break;
    }
    return QVariant();
}
bool DiceResult::contains(Die* die, const std::function<bool(const Die*, const Die*)> equal)
{
    for(auto& value : m_diceValues)
    {
        if(equal(value, die))
        {
            return true;
        }
    }
    return false;
}
qreal DiceResult::getScalarResult()
{
    if(m_diceValues.size() == 1)
    {
        return m_diceValues[0]->getValue();
    }
    else
    {
        qint64 scalar= 0;
        int i= 0;
        for(auto& tmp : m_diceValues)
        {
            if(i > 0)
            {
                switch(m_operator)
                {
                case Dice::ArithmeticOperator::PLUS:
                    scalar+= tmp->getValue();
                    break;
                case Dice::ArithmeticOperator::MULTIPLICATION:
                    scalar*= tmp->getValue();
                    break;
                case Dice::ArithmeticOperator::MINUS:
                    scalar-= tmp->getValue();
                    break;
                case Dice::ArithmeticOperator::POW:
                    scalar= static_cast<int>(pow(static_cast<double>(scalar), static_cast<double>(tmp->getValue())));
                    break;
                case Dice::ArithmeticOperator::DIVIDE:
                case Dice::ArithmeticOperator::INTEGER_DIVIDE:
                    if(tmp->getValue() != 0)
                    {
                        scalar/= tmp->getValue();
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
                scalar= tmp->getValue();
            }
            ++i;
        }
        return scalar;
    }
}

Dice::ArithmeticOperator DiceResult::getOperator() const
{
    return m_operator;
}

void DiceResult::clear()
{
    m_diceValues.clear();
}

void DiceResult::setOperator(const Dice::ArithmeticOperator& dieOperator)
{
    m_operator= dieOperator;
}
QString DiceResult::toString(bool wl)
{
    QStringList scalarSum;
    for(auto& die : m_diceValues)
    {
        scalarSum << QString::number(die->getValue());
    }
    if(wl)
    {
        return QStringLiteral("%3 [label=\"DiceResult Value %1 dice %2\"]")
            .arg(QString::number(getScalarResult()), scalarSum.join('_'), m_id);
    }
    else
    {
        return m_id;
    }
}
Result* DiceResult::getCopy() const
{
    auto copy= new DiceResult();
    copy->setHomogeneous(m_homogeneous);
    copy->setOperator(m_operator);
    copy->m_id= m_id;
    QList<Die*> list;
    for(auto die : m_diceValues)
    {
        auto newdie= new Die(*die);
        newdie->setDisplayed(false);
        // die->displayed();
        list.append(newdie);
    }
    copy->setResultList(list);
    copy->setPrevious(getPrevious());
    return copy;
}
