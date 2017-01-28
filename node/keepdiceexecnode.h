/*************************************************************************
 *   Copyright (C) 2009 by Renaud Guezennec                              *
 *                                                                       *
 *   http://www.rolisteam.org/                                           *
 *                                                                       *
 *   rolisteam is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published   *
 *   by the Free Software Foundation; either version 2 of the License,   *
 *   or (at your option) any later version.                              *
 *                                                                       *
 *   This program is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with this program; if not, write to the                       *
 *   Free Software Foundation, Inc.,                                     *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *************************************************************************/
#ifndef KEEPDICEEXECNODE_H
#define KEEPDICEEXECNODE_H

#include "executionnode.h"
#include "result/diceresult.h"
/**
 * @brief The KeepDiceExecNode class splits the dice result to get the m_numberOfDice dice from the beginning of the dice result.
 * Usually the pervious node of an KeepDiceExecNode is an SortNode.
 */
class KeepDiceExecNode : public ExecutionNode
{
public:
    KeepDiceExecNode();
    virtual ~KeepDiceExecNode();

    virtual void run(ExecutionNode *previous);
    virtual void setDiceKeepNumber(quint64 );
	virtual QString toString(bool)const;
    virtual qint64 getPriority() const;
    virtual ExecutionNode *getCopy() const;
private:
    quint64 m_numberOfDice;
    DiceResult* m_diceResult;
};

#endif // KEEPDICEEXECNODE_H
