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
#ifndef PARENTHESESNODE_H
#define PARENTHESESNODE_H

#include "executionnode.h"
/**
 * @brief The ParenthesesNode class is an ExecutionNode. It is dedicated to manage expression which was inside ParenthesesNode.
 * It is acting just like an StartingNode by for an internal execution tree.
 */
class ParenthesesNode : public ExecutionNode
{
public:
    ParenthesesNode();
    virtual void run(ExecutionNode* previous = NULL);

    void setInternelNode(ExecutionNode* node);
	virtual QString toString(bool)const;
    virtual qint64 getPriority() const;
    virtual ExecutionNode *getCopy() const;
private:
    ExecutionNode* m_internalNode;
};

#endif // PARENTHESESNODE_H
