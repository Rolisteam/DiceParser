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
#include "mergenode.h"

MergeNode::MergeNode() : m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}
void MergeNode::run(ExecutionNode* previous)
{
    if(nullptr == previous)
    {
        m_errors.insert(Dice::ERROR_CODE::NO_PREVIOUS_ERROR, QObject::tr("No previous node before Merge operator"));
        return;
    }

    m_previousNode= previous;
    m_result->setPrevious(previous->getResult());
    ExecutionNode* previousLast= nullptr;
    std::vector<Result*> pastResult;
    for(auto start : *m_startList)
    {
        ExecutionNode* last= getLatestNode(start);
        if(nullptr == last)
            continue;

        auto startResult= start->getResult();
        if(nullptr == startResult)
            continue;

        if(nullptr != previousLast)
        {
            startResult->setPrevious(previousLast->getResult());
            previousLast->setNextNode(start);
        }

        previousLast= last;
        Result* tmpResult= last->getResult();
        while(nullptr != tmpResult)
        {
            DiceResult* dice= dynamic_cast<DiceResult*>(tmpResult);
            if(nullptr != dice)
            {
                ///@todo TODO improve here to set homogeneous while is really
                m_diceResult->setHomogeneous(false);
                for(auto& die : dice->getResultList())
                {
                    if(!m_diceResult->getResultList().contains(die) && (!die->hasBeenDisplayed()))
                    {
                        Die* tmpdie= new Die(*die);
                        die->displayed();
                        m_diceResult->getResultList().append(tmpdie);
                    }
                }
            }
            auto it= std::find_if(pastResult.begin(), pastResult.end(),
                                  [tmpResult](const Result* a) { return (a == tmpResult->getPrevious()); });
            if(it == pastResult.end())
            {
                pastResult.push_back(previousLast->getResult());
                tmpResult= tmpResult->getPrevious();
            }
            else
            {
                tmpResult->setPrevious(nullptr);
                tmpResult= nullptr;
            }
        }
    }

    auto first= m_startList->front();
    m_startList->clear();
    m_startList->push_back(first);

    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}
ExecutionNode* MergeNode::getLatestNode(ExecutionNode* node)
{
    ExecutionNode* next= node;
    while(nullptr != next->getNextNode() && (next->getNextNode() != this))
    {
        next= next->getNextNode();
    }
    return next;
}
QString MergeNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return QString("%1 [label=\"Merge Node\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
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
    MergeNode* node= new MergeNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

std::vector<ExecutionNode*>* MergeNode::getStartList() const
{
    return m_startList;
}

void MergeNode::setStartList(std::vector<ExecutionNode*>* startList)
{
    m_startList= startList;
}
