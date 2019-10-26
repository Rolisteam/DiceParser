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
#include "parenthesesnode.h"

ParenthesesNode::ParenthesesNode() : m_internalNode(nullptr) {}
void ParenthesesNode::setInternelNode(ExecutionNode* node)
{
    m_internalNode= node;
}
void ParenthesesNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(nullptr != m_internalNode)
    {
        m_internalNode->run(this);
        ExecutionNode* temp= m_internalNode;
        while(nullptr != temp->getNextNode())
        {
            temp= temp->getNextNode();
        }
        m_result= temp->getResult();
    }

    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString ParenthesesNode::toString(bool b) const
{
    if(b)
    {
        return QString("%1 [label=\"ParenthesesNode\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}
qint64 ParenthesesNode::getPriority() const
{
    qint64 priority= 3;
    return priority;
}
ExecutionNode* ParenthesesNode::getCopy() const
{
    ParenthesesNode* node= new ParenthesesNode();
    if(nullptr != m_internalNode)
    {
        node->setInternelNode(m_internalNode->getCopy());
    }
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

void ParenthesesNode::generateDotTree(QString& s)
{
    auto str= toString(true);
    if(s.contains(str))
        return;
    s.append(str);
    s.append(";\n");

    if(nullptr != m_internalNode)
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_internalNode->toString(false));
        s.append("[label=\"internal\"];\n");
        m_internalNode->generateDotTree(s);
    }

    if(nullptr != m_nextNode)
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_nextNode->toString(false));
        s.append(" [label=\"next\"];\n");
        //        s.append(" [label=\"nextNode\"];\n");
        m_nextNode->generateDotTree(s);
    }
    else
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append("nullptr;\n");
    }
    if(nullptr != m_result)
    {
        s.append(toString(false));
        s.append(" ->");
        s.append(m_result->toString(false));
        s.append(" [label=\"Result\", style=\"dashed\"];\n");
        if(nullptr == m_nextNode)
            m_result->generateDotTree(s);
    }
}
