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
#ifndef BOOLEANCONDITION_H
#define BOOLEANCONDITION_H

#include <Qt>
#include "validator.h"
/**
 * @brief The BooleanCondition class is a Validator class checking validity from logic expression.
 * It manages many operators (see : @ref LogicOperator).
 */
class BooleanCondition : public Validator
{
public:
    enum LogicOperator { Equal, GreaterThan, LesserThan, GreaterOrEqual, LesserOrEqual, Different};
    BooleanCondition();

    virtual qint64 hasValid(Die* b,bool recursive, bool unhighlight = false) const;

    void setOperator(LogicOperator m);
    void setValue(qint64);
    QString toString();

    virtual quint64 getValidRangeSize(quint64 faces) const;
    /**
     * @brief getCopy
     * @return
     */
    virtual Validator* getCopy() const;

private:
    LogicOperator m_operator;
    qint64 m_value;
};

#endif // BOOLEANCONDITION_H
