/***************************************************************************
 *   Copyright (C) 2015 by Renaud Guezennec                                *
 *   http://www.rolisteam.org/contact                   *
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

#include "operationcondition.h"

OperationCondition::OperationCondition() : m_operator(Modulo), m_boolean(nullptr), m_value(nullptr) {}
OperationCondition::~OperationCondition()
{
    if(m_value != nullptr)
    {
        delete m_value;
        m_value= nullptr;
    }
}
BooleanCondition* OperationCondition::getBoolean() const
{
    return m_boolean;
}

void OperationCondition::setBoolean(BooleanCondition* boolean)
{
    m_boolean= boolean;
}

qint64 OperationCondition::hasValid(Die* b, bool recursive, bool unhighlight) const
{
    QList<qint64> listValues;
    if(recursive)
    {
        listValues= b->getListValue();
    }
    else
    {
        listValues.append(b->getLastRolledValue());
    }

    qint64 sum= 0;
    for(qint64& value : listValues)
    {
        switch(m_operator)
        {
        case Modulo:
        {
            Die die;
            die.setMaxValue(b->getMaxValue());
            auto valueScalar= valueToScalar();
            if(valueScalar == 0)
                valueScalar= 1;
            die.insertRollValue(value % valueScalar);
            sum+= m_boolean->hasValid(&die, recursive, false);
        }
        break;
        }
    }
    if((unhighlight) && (sum == 0))
    {
        b->setHighlighted(false);
    }
    else
    {
        b->setHighlighted(true);
    }

    return sum;
}

void OperationCondition::setOperator(ConditionOperator m)
{
    m_operator= m;
}

void OperationCondition::setValueNode(ExecutionNode* node)
{
    m_value= node;
}

QString OperationCondition::toString()
{
    QString str("");
    switch(m_operator)
    {
    case Modulo:
        str.append(QStringLiteral("\\%"));
        break;
    }
    return QStringLiteral("[%1%2%3]").arg(str).arg(valueToScalar()).arg(m_boolean->toString());
}
bool OperationCondition::isValidRangeSize(std::pair<qint64, qint64>) const
{
    auto value= valueToScalar();
    bool valid= true;

    if(value == 0)
        valid= false;
    /*  else if(nullptr != m_boolean)
          valid = m_boolean->isValidRangeSize(range);*/

    return valid;
}
Validator* OperationCondition::getCopy() const
{
    OperationCondition* val= new OperationCondition();
    val->setOperator(m_operator);
    val->setValueNode(m_value->getCopy());
    BooleanCondition* boolean= dynamic_cast<BooleanCondition*>(m_boolean->getCopy());
    val->setBoolean(boolean);
    return val;
}

qint64 OperationCondition::valueToScalar() const
{
    if(m_value == nullptr)
        return 0;

    m_value->run(nullptr);
    auto result= m_value->getResult();
    return result->getResult(Result::SCALAR).toInt();
}
