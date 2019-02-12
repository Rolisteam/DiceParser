/*************************************************************************
 *   Copyright (C) 2009 by Renaud Guezennec                              *
 *                                                                       *
 *   http://www.rolisteam.org/                                           *
 *                                                                       *
 *   rolisteam is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published   *
 *   by the Free Software Foundation; either version 2 of the License,   *
 *   or (at your option) any later version.                              *
 *                                                                       *
 *   This program is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with this program; if not, write to the                       *
 *   Free Software Foundation, Inc.,                                     *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *************************************************************************/
#include <QList>

#include "keepdiceexecnode.h"

KeepDiceExecNode::KeepDiceExecNode() : m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}
KeepDiceExecNode::~KeepDiceExecNode() {}
void KeepDiceExecNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(nullptr == previous)
    {
        return;
    }
    DiceResult* previousDiceResult= dynamic_cast<DiceResult*>(previous->getResult());
    m_result->setPrevious(previousDiceResult);
    if(nullptr != previousDiceResult)
    {
        QList<Die*> diceList= previousDiceResult->getResultList();

        if(m_numberOfDice < 0)
        {
            m_numberOfDice= diceList.size() + m_numberOfDice;
        }

        QList<Die*> diceList3= diceList.mid(0, static_cast<int>(m_numberOfDice));
        QList<Die*> diceList2;

        for(Die* die : diceList3)
        {
            Die* tmpdie= new Die();
            *tmpdie= *die;
            diceList2.append(tmpdie);
            die->displayed();
        }

        if(m_numberOfDice > static_cast<qint64>(diceList.size()))
        {
            m_errors.insert(TOO_MANY_DICE, QObject::tr(" You ask to keep %1 dice but the result only has %2")
                                               .arg(m_numberOfDice)
                                               .arg(diceList.size()));
        }

        for(auto& tmp : diceList.mid(static_cast<int>(m_numberOfDice), -1))
        {
            tmp->setHighlighted(false);
        }

        m_diceResult->setResultList(diceList2);
        if(nullptr != m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
}
void KeepDiceExecNode::setDiceKeepNumber(qint64 n)
{
    m_numberOfDice= n;
}
QString KeepDiceExecNode::toString(bool wl) const
{
    if(wl)
    {
        return QString("%1 [label=\"KeepDiceExecNode %2\"]").arg(m_id).arg(m_numberOfDice);
    }
    else
    {
        return m_id;
    }
}
qint64 KeepDiceExecNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}

ExecutionNode* KeepDiceExecNode::getCopy() const
{
    KeepDiceExecNode* node= new KeepDiceExecNode();
    node->setDiceKeepNumber(m_numberOfDice);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
