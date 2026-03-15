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
#include "uniquenode.h"

UniqueNode::UniqueNode() : ExecutionNode("%1 [label=\"UniqueNode Node\"]"), m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}
void UniqueNode::run(ExecutionNode* previous)
{
    if(isValid(!previous, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No Previous node")))
        return;
    m_previousNode= previous;

    m_result->setPrevious(previous->getResult());
    Result* tmpResult= previous->getResult();
    if(isValid(!tmpResult, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid result")))
        return;

    DiceResult* dice= dynamic_cast<DiceResult*>(tmpResult);
    if(isValid(!dice, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid dice result")))
        return;

    auto const& resultList= dice->getResultList();
    std::vector<qint64> formerValues;
    formerValues.reserve(resultList.size());
    for(auto& oldDie : resultList)
    {
        auto value= oldDie->getValue();
        auto it= std::find(formerValues.begin(), formerValues.end(), value);

        if(it == formerValues.end())
        {
            auto die= new Die(*oldDie);
            m_diceResult->insertResult(die);
            formerValues.push_back(value);
        }
        oldDie->displayed();
    }
}

qint64 UniqueNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_nextNode)
    {
        priority= m_nextNode->getPriority();
    }
    return priority;
}
ExecutionNode* UniqueNode::getCopy() const
{
    UniqueNode* node= new UniqueNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
