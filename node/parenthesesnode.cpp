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
#include "parenthesesnode.h"

ParenthesesNode::ParenthesesNode()
{

}
void ParenthesesNode::setInternelNode(ExecutionNode* node)
{
    m_internalNode = node;
}
void ParenthesesNode::run(ExecutionNode* /*previous*/)
{
	m_previousNode = NULL;
    if(NULL!=m_internalNode)
    {
        m_internalNode->run(this);
        ExecutionNode* temp=m_internalNode;
       while(NULL!=temp->getNextNode())
       {
            temp=temp->getNextNode();
       }
       m_result = temp->getResult();
    }


    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString ParenthesesNode::toString() const
{
    return "ParenthesesNode";
}
qint64 ParenthesesNode::getPriority() const
{
    qint64 priority=3;
    return priority;
}
