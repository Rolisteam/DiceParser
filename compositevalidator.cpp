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
#include "compositevalidator.h"

CompositeValidator::CompositeValidator()
    : m_operators(nullptr),m_validatorList(nullptr)
{
}

CompositeValidator::~CompositeValidator()
{
    qDeleteAll(*m_validatorList);
    if(nullptr!=m_operators)
    {
        delete m_operators;
    }
}
qint64 CompositeValidator::hasValid(Die* b,bool recursive,bool unhighlight) const
{

    int i = 0;
    qint64 sum = 0;
    bool highLight = false;
    for(const Validator* validator: *m_validatorList)
    {
        qint64 val = validator->hasValid(b,recursive,unhighlight);
        if(i==0)
        {
            sum = val;
            if(b->isHighlighted())
            {
                highLight = b->isHighlighted();
            }
        }
        else
        {
            switch(m_operators->at(i-1))
            {
            case OR:
                sum |= val;

                if(highLight)
                {
                    b->setHighlighted(highLight);
                }
                break;
            case EXCLUSIVE_OR:
                sum ^= val;/// @todo may required to be done by hand
                break;
            case AND:
                sum &= val;
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
    QString str="";
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
bool CompositeValidator::isValidRangeSize(std::pair<qint64,qint64> range) const
{
    bool val = true;
    int i = -1;
    for(Validator* tmp :*m_validatorList)
    {
        quint64 rel = tmp->isValidRangeSize(range);
        val |= rel;
        ++i;
    }

    return val;
}
void CompositeValidator::setOperationList(QVector<LogicOperation>* m)
{
    m_operators = m;
}

void CompositeValidator::setValidatorList(QList<Validator*>* m)
{
    m_validatorList = m;
}
Validator* CompositeValidator::getCopy() const
{
    QVector<LogicOperation>* vector = new QVector<LogicOperation>();
    *vector = *m_operators;

    QList<Validator*>* list= new QList<Validator*>();
    for(auto val : *m_validatorList)
    {
        list->append(val->getCopy());
    }

    CompositeValidator* val = new CompositeValidator();
    val->setOperationList(vector);
    val->setValidatorList(list);
    return val;
}
