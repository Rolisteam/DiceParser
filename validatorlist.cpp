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
#include "validatorlist.h"

#include "diceresult.h"
#include "result.h"
#include "validator.h"
#include <utility>

void mergeResultsAsAND(const ValidatorResult& diceList, ValidatorResult& result)
{
    ValidatorResult val;
    for(auto dice : diceList.validDice())
    {
        if(result.contains(dice.first) || diceList.allTrue())
            val.appendValidDice(dice.first, dice.second);
    }
    result= val;
    result.setAllTrue(diceList.allTrue() & result.allTrue());
}

void mergeResultsAsExeclusiveOR(const ValidatorResult& diceList, ValidatorResult& result)
{
    ValidatorResult val;
    for(auto dice : diceList.validDice())
    {
        if(!result.contains(dice.first))
            val.appendValidDice(dice.first, dice.second);
    }
    result= val;
    result.setAllTrue(diceList.allTrue() ^ result.allTrue());
}

DiceResult* getDiceResult(Result* result)
{
    auto dice= dynamic_cast<DiceResult*>(result);
    if(nullptr == dice)
    {
        qFatal("Error, no dice result");
        // TODO: manage error here.
    }
    return dice;
}

//////////////////////////////////
/// \brief ValidatorResult::ValidatorResult
///
///
///
/// ////////////////////////////////
ValidatorResult::ValidatorResult() {}

const std::vector<std::pair<Die*, qint64>>& ValidatorResult::validDice() const
{
    return m_validDice;
}

std::vector<std::pair<Die*, qint64>>& ValidatorResult::validDiceRef()
{
    return m_validDice;
}

void ValidatorResult::appendValidDice(Die* die, qint64 sum)
{
    m_validDice.push_back(std::make_pair(die, sum));
}

void ValidatorResult::setAllTrue(bool allTrue)
{
    m_allTrue= allTrue;
}

bool ValidatorResult::allTrue() const
{
    return m_allTrue;
}

bool ValidatorResult::contains(Die* die)
{
    auto it= std::find_if(m_validDice.begin(), m_validDice.end(),
                          [die](const std::pair<Die*, qint64>& pair) { return pair.first == die; });

    return it != m_validDice.end();
}

////////////////////////////////////
/// \brief ValidatorList::ValidatorList
///***
///
////////////////////////////////////

ValidatorList::ValidatorList() {}

ValidatorList::~ValidatorList()
{
    qDeleteAll(m_validatorList);
}
qint64 ValidatorList::hasValid(Die* b, bool recursive, bool unhighlight) const
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

QString ValidatorList::toString()
{
    QString str= "";
    /*switch (m_operator)
    {
    case Equal:
        str.append("=");
        break;
    case GreaterThan:
        str.append(">");
        break;
    case LesserThan:
        str.append("<");
        break;
    case GreaterOrEqual:
        str.append(">=");
        break;
    case LesserOrEqual:
        str.append("<=");
        break;
    }
    return QString("[%1%2]").arg(str).arg(m_value);*/
    return str;
}
namespace {
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
} // namespace

Dice::CONDITION_STATE ValidatorList::isValidRangeSize(const std::pair<qint64, qint64>& range) const
{
    std::vector<Dice::CONDITION_STATE> vec;
    std::transform(
        m_validatorList.begin(), m_validatorList.end(), std::back_inserter(vec),
        [range](Validator* validator) -> Dice::CONDITION_STATE { return validator->isValidRangeSize(range); });

    auto itError= std::find(vec.begin(), vec.end(), Dice::CONDITION_STATE::ERROR_STATE);

    if(vec.size() == 1)
        return vec.front();

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

void ValidatorList::setOperationList(const QVector<LogicOperation>& m)
{
    m_operators= m;
}

void ValidatorList::setValidators(const QList<Validator*>& valids)
{
    qDeleteAll(m_validatorList);
    m_validatorList= valids;
}

void ValidatorList::validResult(Result* result, bool recursive, bool unlight,
                                std::function<void(Die*, qint64)> functor) const
{
    std::vector<ValidatorResult> validityData;
    for(auto& validator : m_validatorList)
    {
        ValidatorResult validResult;
        switch(validator->getConditionType())
        {
        case Dice::OnScalar:
        {
            Die die;
            auto scalar= result->getResult(Dice::RESULT_TYPE::SCALAR).toInt();
            die.insertRollValue(scalar);
            if(validator->hasValid(&die, recursive, unlight))
            {
                validResult.setAllTrue(true);
                DiceResult* diceResult= getDiceResult(result);
                if(nullptr == diceResult)
                    break;
                for(auto die : diceResult->getResultList())
                {
                    validResult.appendValidDice(die, die->getValue());
                }
            }
        }
        break;
        case Dice::OnEach:
        {
            DiceResult* diceResult= getDiceResult(result);
            if(nullptr == diceResult)
                break;
            for(auto die : diceResult->getResultList())
            {
                auto score= validator->hasValid(die, recursive, unlight);
                if(score)
                {
                    validResult.appendValidDice(die, score);
                }
            }
        }
        break;
        case Dice::AllOfThem:
        {
            DiceResult* diceResult= getDiceResult(result);
            if(nullptr == diceResult)
                break;
            auto diceList= diceResult->getResultList();
            auto all= std::all_of(diceList.begin(), diceList.end(), [validator, recursive, unlight](Die* die) {
                return validator->hasValid(die, recursive, unlight);
            });
            if(all)
            {
                validResult.setAllTrue(true);
                for(auto die : diceResult->getResultList())
                {
                    validResult.appendValidDice(die, die->getValue());
                }
            }
        }
        break;
        case Dice::OneOfThem:
        {
            DiceResult* diceResult= getDiceResult(result);
            if(nullptr == diceResult)
                break;
            auto diceList= diceResult->getResultList();
            auto any= std::any_of(diceList.begin(), diceList.end(), [validator, recursive, unlight](Die* die) {
                return validator->hasValid(die, recursive, unlight);
            });
            if(any)
            {
                validResult.setAllTrue(true);
                for(auto die : diceResult->getResultList())
                {
                    validResult.appendValidDice(die, die->getValue());
                }
            }
        }
        }
        validityData.push_back(validResult);
    }
    if(validityData.empty())
        return;

    int i= 0;
    ValidatorResult finalResult;

    for(auto vec : validityData)
    {
        auto diceList= vec.validDice();
        if(i == 0)
        {
            std::copy(diceList.begin(), diceList.end(), std::back_inserter(finalResult.validDiceRef()));
        }
        else
        {
            auto id= i - 1;
            if(m_operators.size() <= id)
                continue;

            auto op= m_operators.at(id);
            switch(op)
            {
            case OR:
                std::copy(diceList.begin(), diceList.end(), std::back_inserter(finalResult.validDiceRef()));
                break;
            case AND:
                mergeResultsAsAND(vec, finalResult);
                break;
            case EXCLUSIVE_OR:
                mergeResultsAsExeclusiveOR(vec, finalResult);
                break;
            case NONE:
                break;
            }
        }

        ++i;
    }

    if(finalResult.allTrue())
    {
        DiceResult* diceResult= getDiceResult(result);
        if(nullptr == diceResult)
            return;
        auto diceList= diceResult->getResultList();
        std::transform(diceList.begin(), diceList.end(), std::back_inserter(finalResult.validDiceRef()), [](Die* die) {
            return std::pair<Die*, qint64>({die, 0});
        });
    }

    for(auto die : finalResult.validDice())
    {
        functor(die.first, die.second);
    }
}

ValidatorList* ValidatorList::getCopy() const
{
    ValidatorList* val= new ValidatorList();
    val->setOperationList(m_operators);
    val->setValidators(m_validatorList);
    return val;
}
