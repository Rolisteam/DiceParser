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
#include "validator.h"

Validator::Validator() {}
Validator::~Validator() {}

template <typename Functor>
qint64 Validator::onEach(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const
{
    qint64 result= 0;
    std::for_each(b.begin(), b.end(), [this, recursive, unlight, functor, &result](Die* die) {
        if(hasValid(die, recursive, unlight))
        {
            ++result;
            functor(die, recursive, unlight);
        }
    });
    return result;
}

template <typename Functor>
qint64 Validator::onEachValue(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const
{
    qint64 result= 0;
    std::for_each(b.begin(), b.end(), [this, recursive, unlight, functor, &result](Die* die) {
        if(hasValid(die, recursive, unlight))
        {
            ++result;
            functor(die, recursive, unlight);
        }
    });
    return result;
}

template <typename Functor>
qint64 Validator::oneOfThem(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const
{
    auto oneOfThem= std::any_of(b.begin(), b.end(),
                                [this, recursive, unlight](Die* die) { return hasValid(die, recursive, unlight); });
    if(oneOfThem)
        functor(recursive, unlight);
    return oneOfThem ? 1 : 0;
}

template <typename Functor>
qint64 Validator::allOfThem(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const
{
    auto all= std::all_of(b.begin(), b.end(),
                          [this, recursive, unlight](Die* die) { return hasValid(die, recursive, unlight); });
    if(all)
        functor(recursive, unlight);
    return all ? 1 : 0;
}

template <typename Functor>
qint64 Validator::onScalar(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const
{
    qint64 result= 0;
    for(const auto& die : b)
    {
        result+= die->getValue();
    }
    Die die;
    die.setValue(result);
    if(hasValid(&die, recursive, unlight))
    {
        functor(recursive, unlight);
        return 1;
    }
    return 0;
}

const std::set<qint64>& Validator::getPossibleValues(const std::pair<qint64, qint64>&)
{
    return m_values;
}

template <typename Functor>
qint64 Validator::validResult(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const
{
    qint64 result;
    switch(m_conditionType)
    {
    case Dice::OnEach:
        result= onEach(b, recursive, unlight, functor);
        break;
    case Dice::OnEachValue:
        result= onEachValue(b, recursive, unlight, functor);
        break;
    case Dice::OneOfThem:
        result= oneOfThem(b, recursive, unlight, functor);
        break;
    case Dice::AllOfThem:
        result= allOfThem(b, recursive, unlight, functor);
        break;
    case Dice::OnScalar:
        result= onScalar(b, recursive, unlight, functor);
        break;
    }
    return result;
}

Dice::ConditionType Validator::getConditionType() const
{
    return m_conditionType;
}

void Validator::setConditionType(const Dice::ConditionType& conditionType)
{
    m_conditionType= conditionType;
}
