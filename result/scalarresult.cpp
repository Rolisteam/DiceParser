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
#include "scalarresult.h"

ScalarResult::ScalarResult()
{
    m_resultTypes= Result::SCALAR;
}

void ScalarResult::setValue(qreal i)
{
    m_value= i;
}
QVariant ScalarResult::getResult(Result::RESULT_TYPE type)
{
    if(SCALAR == type)
    {
        return m_value;
    }
    else
        return {};
}
Result* ScalarResult::getCopy() const
{
    auto copy= new ScalarResult();
    copy->setValue(m_value);
    return copy;
}
QString ScalarResult::toString(bool wl)
{
    if(wl)
    {
        return QString("%2 [label=\"ScalarResult %1\"]").arg(m_value).arg(m_id);
    }
    else
    {
        return m_id;
    }
}
