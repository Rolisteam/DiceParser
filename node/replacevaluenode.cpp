/***************************************************************************
 *	Copyright (C) 2021 by Renaud Guezennec                               *
 *   http://www.rolisteam.org/contact                                      *
 *                                                                         *
 *   This software is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "replacevaluenode.h"

#include "diceresult.h"
#include "parsingtoolbox.h"
#include <QDebug>

ReplaceValueNode::ReplaceValueNode() : m_diceResult(new DiceResult)
{
    m_result= m_diceResult;
}

void ReplaceValueNode::setStopAtFirt(bool b)
{
    m_stopAtFirst= b;
}

void ReplaceValueNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(nullptr == previous)
    {
        m_errors.insert(Dice::ERROR_CODE::NO_PREVIOUS_ERROR,
                        QStringLiteral("No previous node before Switch/Case operator"));
        return;
    }
    auto previousResult= previous->getResult();
    m_result->setPrevious(previousResult);

    if(nullptr == previousResult
       || (!previousResult->hasResultOfType(Dice::RESULT_TYPE::SCALAR)
           && !previousResult->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST)))
    {
        m_errors.insert(Dice::ERROR_CODE::NO_VALID_RESULT,
                        QStringLiteral("No scalar or dice result before Switch/Case operator"));
        return;
    }

    QList<Die*> dieList;
    if(previousResult->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
    {
        auto diceResult= dynamic_cast<DiceResult*>(previousResult);
        if(diceResult)
            dieList.append(diceResult->getResultList());
    }

    for(auto die : dieList)
    {
        QStringList resultList;
        for(auto const& info : qAsConst(m_branchList))
        {
            if(info->validatorList)
            {
                auto res= info->validatorList->hasValid(die, false);
                if(!res)
                    continue;
            }
            else if(!resultList.isEmpty())
                break;

            auto replaceValresult= info->node->getResult();
            if(replaceValresult)
                die->replaceLastValue(replaceValresult->getResult(Dice::RESULT_TYPE::SCALAR).toInt());
            break;
        }
        m_diceResult->insertResult(die);
    }

    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}

QString ReplaceValueNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return QString("%1 [label=\"ReplaceValueNode\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}

qint64 ReplaceValueNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}

ExecutionNode* ReplaceValueNode::getCopy() const
{
    ReplaceValueNode* node= new ReplaceValueNode();
    for(auto const& info : qAsConst(m_branchList))
    {
        node->insertCase(info->node, info->validatorList);
    }

    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

void ReplaceValueNode::insertCase(ExecutionNode* node, ValidatorList* validator)
{
    std::unique_ptr<Dice::CaseInfo> info(new Dice::CaseInfo{validator, node});
    m_branchList.push_back(std::move(info));
}
