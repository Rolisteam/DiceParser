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

using InstructionSet= std::vector<ExecutionNode*>;

QStringList allFirstResultAsString(std::vector<InstructionSet> startingNodes, bool& hasAlias)
{
    // QStringList allResult;
    QStringList stringListResult;
    for(auto node : startingNodes)
    {
        QVariant var;
        if(ParsingToolBox::hasResultOfType(Dice::RESULT_TYPE::STRING, node, var))
        {
            stringListResult << var.toString();
            hasAlias= true;
        }
        else if(hasResultOfType(Dice::RESULT_TYPE::SCALAR, node, var, true))
        {
            stringListResult << QString::number(var.toReal());
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
        auto list= allFirstResultAsString(m_startingNodes, true);
        // auto string= new StringResult();
        // QStringList list;
        /*std::for_each(resultVec.begin(), resultVec.end(), [&list](Result* result) {
            auto value= result->getResult(Dice::RESULT_TYPE::SCALAR).toDouble();
            auto diceList= result->getResult(Dice::RESULT_TYPE::DICE_LIST).value<QList<Die*>>();
            auto string= result->getResult(Dice::RESULT_TYPE::STRING).toString();

            if(!string.isEmpty())
                list.append(string);
            else
            {
                QStringList diceStr;
                std::transform(diceList.begin(), diceList.end(), std::back_inserter(diceStr), [](Die* die) {
                    auto values= die->getListValue();

                    QStringList valuesStr;
                    std::transform(values.begin(), values.end(), std::back_inserter(valuesStr),
                                   [](qint64 val) { return QString::number(val); });

                    if(valuesStr.size() == 1)
                        return QStringLiteral("%1").arg(die->getValue());
                    else
                        return QStringLiteral("%1 [%2]").arg(die->getValue()).arg(valuesStr.join(","));
                });
                list.append(QStringLiteral("%1 - Details [%2]").arg(value).arg(diceStr.join(",")));
            }
        });
        string->addText(list.join('\n'));
        string->finished();
        m_result= string;*/
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
