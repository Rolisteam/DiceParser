/***************************************************************************
    *   Copyright (C) 2015 by Renaud Guezennec                                *
    *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
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

#include <Qt>
#include "validator.h"
#include "booleancondition.h"

class OperationCondition : public Validator
{
public:
    enum ConditionOperator { Modulo };
    OperationCondition();

    virtual qint64 hasValid(Die* b,bool recursive, bool unhighlight = false) const;

    void setOperator(ConditionOperator m);
    void setValue(qint64);
    QString toString();

    virtual quint64 getValidRangeSize(quint64 faces) const;

    BooleanCondition *getBoolean() const;
    void setBoolean(BooleanCondition *boolean);


private:
    ConditionOperator m_operator;
    BooleanCondition* m_boolean;
    qint64 m_value;
};

#endif // OPERATIONCONDITION_H
