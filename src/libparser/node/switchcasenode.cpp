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
#include "switchcasenode.h"

#include "stringresult.h"
#include <QDebug>
#include <diceparser/parsingtoolbox.h>
#include <memory>

SwitchCaseNode::SwitchCaseNode() : m_stringResult(new StringResult)
{
    m_result= m_stringResult;
}

void SwitchCaseNode::setStopAtFirt(bool b)
{
    m_stopAtFirst= b;
}

void SwitchCaseNode::run(ExecutionNode* previous)
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

    auto diceResult= dynamic_cast<DiceResult*>(previousResult);

    QStringList finalResultList;
    if(diceResult)
    {
        for(auto die : diceResult->getResultList())
        {
            QStringList resultList;
            for(auto const& info : qAsConst(m_branchList))
            {
                if(m_stopAtFirst && !resultList.isEmpty())
                    break;
                if(info->validatorList)
                {
                    if(info->validatorList->hasValid(die, true))
                    {
                        auto lastNode= ParsingToolBox::getLeafNode(info->node);
                        if(lastNode && lastNode->getResult())
                        {
                            resultList << lastNode->getResult()->getStringResult();
                        }
                    }
                }
                else if(resultList.isEmpty())
                {
                    info->node->run(m_previousNode);
                    auto lastNode= ParsingToolBox::getLeafNode(info->node);
                    if(lastNode && lastNode->getResult())
                    {
                        resultList << lastNode->getResult()->getStringResult();
                    }
                    else
                        resultList << QString();
                }
            }
            finalResultList << resultList;
        }
    }
    else
    {
        auto scalar= previousResult->getResult(Dice::RESULT_TYPE::SCALAR).toReal();
        for(auto const& info : qAsConst(m_branchList))
        {
            if(m_stopAtFirst && !finalResultList.isEmpty())
                break;

            if(info->validatorList)
            {
                auto die= std::make_unique<Die>();
                die->insertRollValue(scalar);
                if(info->validatorList->hasValid(die.get(), true))
                {
                    auto lastNode= ParsingToolBox::getLeafNode(info->node);
                    if(lastNode && lastNode->getResult())
                    {
                        finalResultList << lastNode->getResult()->getStringResult();
                    }
                }
            }
            else if(finalResultList.isEmpty())
            {
                info->node->run(m_previousNode);
                auto lastNode= ParsingToolBox::getLeafNode(info->node);
                if(lastNode && lastNode->getResult())
                {
                    finalResultList << lastNode->getResult()->getStringResult();
                }
                else
                    finalResultList << QString();
            }
        }
    }

    for(auto const& str : qAsConst(finalResultList))
        m_stringResult->addText(str);

    if(m_stringResult->getText().isEmpty())
        m_errors.insert(Dice::ERROR_CODE::NO_VALID_RESULT, QStringLiteral("No value fits the Switch/Case operator"));

    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}

QString SwitchCaseNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return QString("%1 [label=\"SwitchCaseNode\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}

qint64 SwitchCaseNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}

ExecutionNode* SwitchCaseNode::getCopy() const
{
    SwitchCaseNode* node= new SwitchCaseNode();
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

void SwitchCaseNode::insertCase(ExecutionNode* node, ValidatorList* validator)
{
    std::unique_ptr<Dice::CaseInfo> info(new Dice::CaseInfo{validator, node});
    m_branchList.push_back(std::move(info));
}
