/***************************************************************************
 * Copyright (C) 2014 by Renaud Guezennec                                   *
 * https://rolisteam.org/contact                      *
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
#include "bind.h"

BindNode::BindNode() : ExecutionNode("%1 [label=\"Bind Node\"]"), m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}
void BindNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(nullptr == m_previousNode)
        return;

    if(!m_startList)
        return;

    m_result->setPrevious(previous->getResult());
    for(auto start : *m_startList)
    {
        ExecutionNode* last= getLatestNode(start);
        if(!last)
            continue;

        auto tmpResult= last->getResult();
        QSet<Result*> alreadyVisited;
        while(nullptr != tmpResult && !alreadyVisited.contains(tmpResult))
        {
            alreadyVisited.insert(tmpResult);
            DiceResult* dice= dynamic_cast<DiceResult*>(tmpResult);

            if(nullptr != dice)
            {
                m_diceResult->setHomogeneous(false);
                auto list= dice->getResultList();
                for(int i= 0; i < list.size(); ++i)
                {
                    auto die= list[i];

                    if(!die)
                        continue;

                    if(!die->hasBeenDisplayed())
                    {
                        Die* tmpdie= new Die(*die);
                        die->displayed();
                        m_diceResult->getResultList().append(tmpdie);
                    }
                }
            }
            tmpResult= tmpResult->getPrevious();
        }
    }
}
ExecutionNode* BindNode::getLatestNode(ExecutionNode* node)
{
    ExecutionNode* next= node;
    while(nullptr != next->getNextNode() && (next->getNextNode() != this))
    {
        next= next->getNextNode();
    }
    return next;
}

qint64 BindNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}
ExecutionNode* BindNode::getCopy() const
{
    BindNode* node= new BindNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

std::vector<ExecutionNode*>* BindNode::getStartList() const
{
    return m_startList;
}

void BindNode::setStartList(std::vector<ExecutionNode*>* startList)
{
    m_startList= startList;
}
