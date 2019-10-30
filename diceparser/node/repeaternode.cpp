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

RepeaterNode::RepeaterNode() {}

void RepeaterNode::run(ExecutionNode* previousNode)
{
    m_previousNode= previousNode;
    m_times->run(this);
    m_times= ParsingToolBox::getLeafNode(m_times);
    auto times= m_times->getResult();
    auto timeCount= times->getResult(Dice::RESULT_TYPE::SCALAR).toInt();
    auto cmd= m_cmd->getCopy();
    for(int i= 0; i < timeCount; ++i)
    {
        m_cmd->run(this);
        auto end= ParsingToolBox::getLeafNode(cmd);
        auto result= end->getResult();

        if(nullptr == result)
            continue;

        if(result->hasResultOfType(Dice::RESULT_TYPE::SCALAR))
        {
            if(m_sumAll)
            {
                auto res= new ScalarResult();

                m_result= res;
            }
            else
            {
                auto res= new StringResult();
                m_result= res;
            }
        }
        else if(result->hasResultOfType(Dice::RESULT_TYPE::STRING))
        {
            auto res= new StringResult();

            m_result= res;
        }
        else if(result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
        {
            if(m_sumAll)
            {
                auto res= new ScalarResult();

                m_result= res;
            }
            else
            {
                auto res= new StringResult();

                m_result= res;
            }
        }

        cmd= m_cmd->getCopy();
    }
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

void RepeaterNode::setCommand(ExecutionNode* cmd)
{
    m_cmd.reset(cmd);
}

void RepeaterNode::setTimeNode(ExecutionNode* time)
{
    m_times= time;
}

void RepeaterNode::setSumAll(bool b)
{
    m_sumAll= b;
}
