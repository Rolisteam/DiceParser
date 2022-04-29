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
#include "compositevalidator.h"

#include <map>

CompositeValidator::CompositeValidator() {}

CompositeValidator::~CompositeValidator()
{
    qDeleteAll(m_validatorList);
}
qint64 CompositeValidator::hasValid(Die* b, bool recursive, bool unhighlight) const
{
    int i= 0;
    qint64 sum= 0;
    bool highLight= false;
    for(auto& validator : m_validatorList)
    {
        qint64 val= validator->hasValid(b, recursive, unhighlight);
        if(i == 0)
        {
            sum= val;
            if(b->isHighlighted())
            {
                highLight= b->isHighlighted();
            }
        }
        else
        {
            switch(m_operators.at(i - 1))
            {
            case OR:
                sum|= val;

                if(highLight)
                {
                    b->setHighlighted(highLight);
                }
                break;
            case EXCLUSIVE_OR:
                sum^= val; /// @todo may required to be done by hand
                break;
            case AND:
                sum&= val;
                break;
            default:
                break;
            }
        }
        ++i;
    }

    return sum;
}

QString CompositeValidator::toString()
{
    // m_validatorList
    QStringList validatorsTextList;
    std::transform(m_validatorList.begin(), m_validatorList.end(), std::back_inserter(validatorsTextList),
                   [](Validator* validator) { return validator->toString(); });
    QStringList operatorTextList;
    std::transform(
        m_operators.begin(), m_operators.end(), std::back_inserter(operatorTextList), [](LogicOperation validator) {
            static std::map<LogicOperation, QString> map({{OR, "|"}, {EXCLUSIVE_OR, "^"}, {AND, "&"}, {NONE, ""}});
            return map[validator];
        });

    if(validatorsTextList.size() - 1 != operatorTextList.size())
        return QString("Error - operator and validator count don't fit");

    QStringList result;
    int i= 0;
    for(auto text : validatorsTextList)
    {
        result << text << operatorTextList[i];

        ++i;
    }
    return result.join(" ");
}

Dice::CONDITION_STATE testAND(Dice::CONDITION_STATE before, Dice::CONDITION_STATE current)
{
    if(before == Dice::CONDITION_STATE::UNREACHABLE || current == Dice::CONDITION_STATE::UNREACHABLE)
        return Dice::CONDITION_STATE::UNREACHABLE;
    else if(before == Dice::CONDITION_STATE::ALWAYSTRUE && current == Dice::CONDITION_STATE::ALWAYSTRUE)
        return Dice::CONDITION_STATE::ALWAYSTRUE;
    else
        return Dice::CONDITION_STATE::REACHABLE;
}

Dice::CONDITION_STATE testOR(Dice::CONDITION_STATE before, Dice::CONDITION_STATE current)
{
    if(before == Dice::CONDITION_STATE::UNREACHABLE && current == Dice::CONDITION_STATE::UNREACHABLE)
        return Dice::CONDITION_STATE::UNREACHABLE;
    else if(before == Dice::CONDITION_STATE::ALWAYSTRUE || current == Dice::CONDITION_STATE::ALWAYSTRUE)
        return Dice::CONDITION_STATE::ALWAYSTRUE;
    else
        return Dice::CONDITION_STATE::REACHABLE;
}

Dice::CONDITION_STATE testXOR(Dice::CONDITION_STATE before, Dice::CONDITION_STATE current)
{
    if(before == current
       && (before == Dice::CONDITION_STATE::UNREACHABLE || before == Dice::CONDITION_STATE::ALWAYSTRUE))
        return Dice::CONDITION_STATE::UNREACHABLE;
    else if((before != current)
            && (before == Dice::CONDITION_STATE::ALWAYSTRUE || before == Dice::CONDITION_STATE::UNREACHABLE)
            && (before != Dice::CONDITION_STATE::REACHABLE || current != Dice::CONDITION_STATE::REACHABLE))
        return Dice::CONDITION_STATE::ALWAYSTRUE;
    else
        return Dice::CONDITION_STATE::REACHABLE;
}

Dice::CONDITION_STATE CompositeValidator::isValidRangeSize(const std::pair<qint64, qint64>& range) const
{
    std::vector<Dice::CONDITION_STATE> vec;
    std::transform(
        m_validatorList.begin(), m_validatorList.end(), std::back_inserter(vec),
        [range](Validator* validator) -> Dice::CONDITION_STATE { return validator->isValidRangeSize(range); });

    auto itError= std::find(vec.begin(), vec.end(), Dice::CONDITION_STATE::ERROR_STATE);

    if((static_cast<int>(vec.size()) != m_operators.size() + 1) || (itError != vec.end()))
    {
        return Dice::CONDITION_STATE::ERROR_STATE;
    }

    std::size_t i= 0;
    Dice::CONDITION_STATE val= Dice::CONDITION_STATE::ERROR_STATE;
    for(const auto& op : m_operators)
    {
        auto currentState= vec[i + 1];
        if(i == 0)
        {
            val= vec[i];
        }
        switch(op)
        {
        case OR:
            val= testAND(val, currentState);
            break;
        case EXCLUSIVE_OR:
            val= testOR(val, currentState);
            break;
        case AND:
            val= testXOR(val, currentState);
            break;
        case NONE:
            val= Dice::CONDITION_STATE::ERROR_STATE;
            break;
        }

        ++i;
    }
    return val;
}

void CompositeValidator::setOperationList(const QVector<LogicOperation>& m)
{
    m_operators= m;
}

void CompositeValidator::setValidatorList(const QList<Validator*>& valids)
{
    qDeleteAll(m_validatorList);
    m_validatorList= valids;
}
Validator* CompositeValidator::getCopy() const
{
    CompositeValidator* val= new CompositeValidator();
    val->setOperationList(m_operators);
    val->setValidatorList(m_validatorList);
    return val;
}
