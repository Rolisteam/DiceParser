/***************************************************************************
 *   Copyright (C) 2015 by Renaud Guezennec                                *
 *   https://rolisteam.org/contact                   *
 *                                                                         *
 *   rolisteam is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef OPERATIONCONDITION_H
#define OPERATIONCONDITION_H

#include "booleancondition.h"
#include "node/executionnode.h"
#include "validator.h"
#include <Qt>

class OperationCondition : public Validator
{
public:
    OperationCondition();
    virtual ~OperationCondition() override;

    virtual qint64 hasValid(Die* b, bool recursive, bool unhighlight= false) const override;

    void setOperator(Dice::ConditionOperator m);
    // void setValue(qint64);
    void setValueNode(ExecutionNode* node);
    QString toString() override;

    virtual Dice::CONDITION_STATE isValidRangeSize(const std::pair<qint64, qint64>& range) const override;

    BooleanCondition* getBoolean() const;
    void setBoolean(BooleanCondition* boolean);

    virtual Validator* getCopy() const override;

    const std::set<qint64>& getPossibleValues(const std::pair<qint64, qint64>& range) override;

private:
    qint64 valueToScalar() const;

private:
    Dice::ConditionOperator m_operator{Dice::ConditionOperator::Modulo};
    BooleanCondition* m_boolean= nullptr;
    // qint64 m_value;
    ExecutionNode* m_value= nullptr;
};

#endif // OPERATIONCONDITION_H
