/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://renaudguezennec.homelinux.org/accueil,3.html                      *
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
#include "booleancondition.h"


BooleanCondition::BooleanCondition()
    : m_operator(Equal)
{
}
qint64 BooleanCondition::hasValid(Die* b,bool recursive,bool unhighlight) const
{
    QList<qint64> listValues;
    if(recursive)
    {
        listValues = b->getListValue();
    }
    else
    {
        listValues.append(b->getLastRolledValue());
    }

    qint64 sum= 0;
    foreach(qint64 value, listValues)
    {
        switch(m_operator)
        {
            case Equal:
                sum+=(value==m_value)?1:0;
                break;
            case GreaterThan:
                sum+= (value>m_value)?1:0;
                break;
            case LesserThan:
                sum+= (value<m_value)?1:0;
                break;
            case GreaterOrEqual:
                sum+= (value>=m_value)?1:0;
                break;
            case LesserOrEqual:
                sum+= (value<=m_value)?1:0;
                break;
        }
    }
    if((unhighlight)&&(sum==0))
    {
        b->setHighlighted(false);
    }
    else
    {
         b->setHighlighted(true);
    }

    return sum;
}

void BooleanCondition::setOperator(LogicOperator m)
{
    m_operator = m;
}

void BooleanCondition::setValue(qint64 v)
{
    m_value=v;
}
QString BooleanCondition::toString()
{
    QString str="";
    switch (m_operator)
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
    return QString("[%1%2]").arg(str).arg(m_value);
}
quint64 BooleanCondition::getValidRangeSize(quint64 faces) const
{
    switch (m_operator)
    {
    case Equal:
        return 1;
    case GreaterThan:
        return faces-m_value;
    case LesserThan:
        return m_value-1;
    case GreaterOrEqual:
        return faces-(m_value-1);
    case LesserOrEqual:
        return m_value;
    }
}
