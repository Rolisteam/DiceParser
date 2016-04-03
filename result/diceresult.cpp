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
#include "diceresult.h"

DiceResult::DiceResult()
{
    m_resultTypes= (DICE_LIST | SCALAR);
    m_homogeneous = true;
}
void DiceResult::insertResult(Die* die)
{
    m_diceValues.append(die);
}
QList<Die*>& DiceResult::getResultList()
{
    return m_diceValues;
}
bool DiceResult::isHomogeneous() const
{
    return m_homogeneous;
}
void DiceResult::setHomogeneous(bool b)
{
    m_homogeneous = b;
}

void DiceResult::setResultList(QList<Die*> list)
{
	qDeleteAll(m_diceValues.begin(), m_diceValues.end());
    m_diceValues.clear();
    m_diceValues << list;
}
DiceResult::~DiceResult()
{
	qDeleteAll(m_diceValues.begin(), m_diceValues.end());
	m_diceValues.clear();
}
QVariant DiceResult::getResult(RESULT_TYPE type)
{
    switch (type)
    {
        case SCALAR:
        {
             return getScalarResult();
        }
        case DICE_LIST:
        {
            return QVariant();
        }
        default:
            break;
    }
    return QVariant();

}
/*bool DiceResult::hasResultOfType(RESULT_TYPE type) const
{
    return (m_resultTypes & type);
}*/
qreal DiceResult::getScalarResult()
{
    if(m_diceValues.size()==1)
    {
        return m_diceValues[0]->getValue();
    }
    else
    {
       qint64 scalarSum = 0;
       foreach(Die* die,m_diceValues)
       {
           scalarSum+=die->getValue();
       }
       return scalarSum;
    }

    return 0;
}
QString DiceResult::toString(bool wl)
{
    QStringList scalarSum;
    foreach(Die* die,m_diceValues)
    {
        scalarSum << QString::number(die->getValue());
    }
	if(wl)
	{
		return QStringLiteral("%3 [label=\"DiceResult Value %1 dice %2\"]").arg(getScalarResult()).arg(scalarSum.join('_')).arg(m_id);
	}
	else
	{
		return m_id;
	}
}
