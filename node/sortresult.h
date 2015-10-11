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
#ifndef SORTRESULT_H
#define SORTRESULT_H

#include "executionnode.h"
#include "result/diceresult.h"
/**
 * @brief The SortResultNode class is an ExecutionNode, and it is dedicated to sort dice list.
 * The sort is made by hand, using half-interval search algorithm.
 */
class SortResultNode : public ExecutionNode
{
public:
    /**
     * @brief SortResultNode
     */
    SortResultNode();
    /**
     * @brief run
     */
    virtual void run(ExecutionNode*);

    /**
     * @brief setSortAscending
     * @param asc
     */
    void setSortAscending(bool asc);
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool wl)const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;

private:
    bool m_ascending;
    DiceResult* m_diceResult;
};

#endif // SORTRESULT_H
