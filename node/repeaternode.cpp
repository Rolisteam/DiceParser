/***************************************************************************
 * Copyright (C) 2019 by Renaud Guezennec                                   *
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
#include "node/repeaternode.h"

#include "diceparserhelper.h"
#include "executionnode.h"
#include "parsingtoolbox.h"
#include "result/stringresult.h"
#include <QDebug>

using InstructionSet= std::vector<ExecutionNode*>;

QStringList allFirstResultAsString(const InstructionSet& startingNodes, bool& hasAlias)
{
    ParsingToolBox parsingBox;
    // QStringList allResult;
    QStringList stringListResult;
    for(auto node : startingNodes)
    {
        auto pair= parsingBox.hasResultOfType(Dice::RESULT_TYPE::STRING, node);
        auto pairStr= parsingBox.hasResultOfType(Dice::RESULT_TYPE::SCALAR, node, true);
        if(pair.first)
        {
            stringListResult << pair.second.toString();
            hasAlias= true;
        }
        else if(pairStr.first)
        {
            stringListResult << QString::number(pairStr.second.toReal());
            hasAlias= true;
        }
    }
    return stringListResult;
}

std::vector<ExecutionNode*> makeCopy(std::vector<ExecutionNode*> cmds)
{
    std::vector<ExecutionNode*> copy;
    std::transform(cmds.begin(), cmds.end(), std::back_inserter(copy),
                   [](ExecutionNode* node) { return node->getCopy(); });
    return copy;
}

RepeaterNode::RepeaterNode() {}

void RepeaterNode::run(ExecutionNode* previousNode)
{
    m_previousNode= previousNode;

    if(nullptr == m_times || m_cmd.empty())
        return;

    m_times->run(this);
    m_times= ParsingToolBox::getLeafNode(m_times);
    auto times= m_times->getResult();
    if(!times)
        return;

    std::vector<InstructionSet> m_startingNodes;
    auto timeCount= times->getResult(Dice::RESULT_TYPE::SCALAR).toInt();
    auto cmd= makeCopy(m_cmd);
    std::vector<Result*> resultVec;
    for(int i= 0; i < timeCount; ++i)
    {
        m_startingNodes.push_back(cmd);
        std::for_each(cmd.begin(), cmd.end(), [this, &resultVec](ExecutionNode* node) {
            node->run(this);
            auto end= ParsingToolBox::getLeafNode(node);
            auto leafResult= end->getResult();

            if(nullptr == leafResult)
                return;

            resultVec.push_back(leafResult);
        });
        cmd= makeCopy(m_cmd);
    }
    if(m_sumAll)
    {
        auto scalar= new ScalarResult();
        qreal value= 0.0;
        std::for_each(resultVec.begin(), resultVec.end(),
                      [&value](Result* result) { value+= result->getResult(Dice::RESULT_TYPE::SCALAR).toDouble(); });
        scalar->setValue(value);
        m_result= scalar;
    }
    else
    {
        auto string= new StringResult();

        QStringList listOfStrResult;
        for(auto instructions : m_startingNodes)
        {
            ParsingToolBox parsingBox;
            parsingBox.setStartNodes(instructions);
            auto finalString= parsingBox.finalStringResult();
            listOfStrResult << finalString;
        }
        if(!listOfStrResult.isEmpty())
            string->addText(listOfStrResult.join('\n'));

        m_result= string;

        qDebug().noquote() << listOfStrResult.join('\n');
    }

    if(nullptr != m_nextNode)
        m_nextNode->run(this);
}

QString RepeaterNode::toString(bool withLabel) const
{
    return withLabel ? QStringLiteral("") : QStringLiteral("");
}

qint64 RepeaterNode::getPriority() const
{
    return 4;
}

ExecutionNode* RepeaterNode::getCopy() const
{
    return nullptr;
}

void RepeaterNode::setCommand(const std::vector<ExecutionNode*>& cmd)
{
    m_cmd= cmd;
}

void RepeaterNode::setTimeNode(ExecutionNode* time)
{
    m_times= time;
}

void RepeaterNode::setSumAll(bool b)
{
    m_sumAll= b;
}
