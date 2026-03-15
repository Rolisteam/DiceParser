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
#include "mergenode.h"

#include <diceparser/parsingtoolbox.h>

MergeNode::MergeNode(std::vector<ExecutionNode*>& startList)
    : ExecutionNode("%1 [label=\"Merge Node\"]"), m_startList(startList), m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}

int findDistance(const std::vector<ExecutionNode*>& startList, MergeNode* node)
{
    int i= 0;
    for(auto start : startList)
    {
        auto temp= start;
        while(temp != nullptr)
        {
            if(temp == node)
                return i;
            temp= temp->getNextNode();
        }
        ++i;
    }
    return i;
}
void MergeNode::run(ExecutionNode* previous)
{
    if(isValid(!previous, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No previous node before Merge operator")))
        return;

    // m_previousNode= previous;
    // m_result->setPrevious(previous->getResult());
    // std::vector<Result*> pastResult;
    if(isValid(m_startList.empty(), Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No several instruction")))
        return;

    auto instId= findDistance(m_startList, this);

    ExecutionNode* previousLast= nullptr;
    std::for_each(std::begin(m_startList), std::end(m_startList),
                  [&previousLast, this](ExecutionNode* node)
                  {
                      ExecutionNode* last= getLatestNode(node);
                      if(previousLast)
                      {
                          previousLast->setNextNode(node);
                          auto resultNode= node->getResult();
                          if(resultNode)
                              resultNode->setPrevious(previousLast->getResult());
                      }
                      previousLast= last;
                  });

    if(instId != 0)
    {
        Result* tmpResult= previous->getResult();
        while(nullptr != tmpResult)
        {
            DiceResult* dice= dynamic_cast<DiceResult*>(tmpResult);
            if(nullptr != dice && dice != m_diceResult)
            {
                m_diceResult->setHomogeneous(false);

                for(auto& die : dice->getResultList())
                {
                    if(!m_diceResult->getResultList().contains(die) && (!die->hasBeenDisplayed()))
                    {
                        Die* tmpdie= new Die(*die);
                        die->displayed();
                        m_diceResult->insertResult(tmpdie);
                    }
                }
            }
            tmpResult= tmpResult->getPrevious();
        }
    }

    auto first= m_startList.front();
    m_startList.clear();
    m_startList.push_back(first);
}
#include <QDebug>
ExecutionNode* MergeNode::getLatestNode(ExecutionNode* node)
{
    ExecutionNode* next= node;
    while(nullptr != next->getNextNode() && (next->getNextNode() != this))
    {
        // qDebug() << "find latest node" << next->toString(true) << next->getNextNode()->toString(true);
        next= next->getNextNode();
    }
    return next;
}

qint64 MergeNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}
ExecutionNode* MergeNode::getCopy() const
{
    MergeNode* node= new MergeNode(m_startList);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

/*void MergeNode::setStartList(const std::vector<ExecutionNode*>& startList)
{
    m_startList= startList;
}*/
