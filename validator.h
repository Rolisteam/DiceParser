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
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "diceparserhelper.h"
#include "die.h"
#include <QString>
#include <Qt>
#include <set>
/**
 * @brief The Validator class is an abstract class for checking the validity of dice for some
 * operator.
 */
// template <Dice::ConditionType C>
class Validator
{
public:
    /**
     * @brief Validator
     */
    Validator();
    /**
     * @brief ~Validator
     */
    virtual ~Validator();
    /**
     * @brief hasValid
     * @param b
     * @param recursive
     * @param unlight
     * @return
     */
    virtual qint64 hasValid(Die* b, bool recursive, bool unlight= false) const= 0;
    /**
     * @brief toString
     * @return
     */
    virtual QString toString()= 0;
    /**
     * @brief getValidRangeSize
     * @param faces
     * @return
     */
    virtual Dice::CONDITION_STATE isValidRangeSize(const std::pair<qint64, qint64>& range) const= 0;
    /**
     * @brief getCopy
     * @return return  a copy of this validator
     */
    virtual Validator* getCopy() const= 0;
    /**
     * @brief getPossibleValues
     * @param range
     * @return
     */
    virtual const std::set<qint64>& getPossibleValues(const std::pair<qint64, qint64>& range);
    /**
     * @brief validResult
     * @param b
     * @param recursive
     * @param unlight
     * @return
     */
    template <typename Functor>
    qint64 validResult(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const;

    Dice::ConditionType getConditionType() const;
    void setConditionType(const Dice::ConditionType& conditionType);

protected:
    template <typename Functor>
    qint64 onEach(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const;
    template <typename Functor>
    qint64 onEachValue(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const;
    template <typename Functor>
    qint64 oneOfThem(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const;
    template <typename Functor>
    qint64 allOfThem(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const;
    template <typename Functor>
    qint64 onScalar(const std::vector<Die*>& b, bool recursive, bool unlight, Functor functor) const;

protected:
    std::set<qint64> m_values;
    Dice::ConditionType m_conditionType= Dice::OnEach;
};

#endif // VALIDATOR_H
