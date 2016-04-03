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
#ifndef SCALARRESULT_H
#define SCALARRESULT_H

#include "result.h"
#include <Qt>

/**
 * @brief The ScalarResult class is used to store scalar result by many ExecutionNode.
 */
class ScalarResult : public Result
{
public:
    /**
     * @brief ScalarResult
     */
    ScalarResult();
    /**
     * @brief getResult
     * @return
     */
    virtual QVariant getResult(Result::RESULT_TYPE);
    /**
     * @brief setValue
     * @param i
     */
    void setValue(qreal i);
    /**
     * @brief toString
     * @return
     */
	virtual QString toString(bool);

private:
    qreal m_value;
};

#endif // SCALARRESULT_H
