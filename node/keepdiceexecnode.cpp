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


KeepDiceExecNode::KeepDiceExecNode()
    : m_diceResult(new DiceResult())
{
    m_result = m_diceResult;
}

void KeepDiceExecNode::run(ExecutionNode* previous)
{
m_previousNode = previous;
    if(NULL==previous)
    {
        return;
    }
    DiceResult* previousDiceResult = static_cast<DiceResult*>(previous->getResult());
    m_result->setPrevious(previousDiceResult);
    if(NULL!=previousDiceResult)
    {
        QList<Die*> diceList=previousDiceResult->getResultList();
        QList<Die*> diceList2 = diceList.mid(0,m_numberOfDice);

        foreach(Die* tmp,diceList.mid(m_numberOfDice,-1))
        {
            tmp->setHighlighted(false);
        }

        m_diceResult->setResultList(diceList2);
        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
}
void KeepDiceExecNode::setDiceKeepNumber(quint64 n)
{
    m_numberOfDice = n;
}
QString KeepDiceExecNode::toString() const
{
    return QString("KeepDiceExecNode [label=\"KeepDiceExecNode %1\"]").arg(m_numberOfDice);
}
qint64 KeepDiceExecNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
