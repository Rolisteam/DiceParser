/*************************************************************************
 *   Copyright (C) 2009 by Renaud Guezennec                              *
 *                                                                       *
 *   https://rolisteam.org/                                           *
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

#include "diceparser/parsingtoolbox.h"
#include "keepdiceexecnode.h"

KeepDiceExecNode::KeepDiceExecNode() : ExecutionNode("%1 [label=\"KeepDiceExecNode\"]"), m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}
KeepDiceExecNode::~KeepDiceExecNode() {}
void KeepDiceExecNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(isValid(!previous || !m_numberOfDiceNode, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid result")))
        return;

    m_numberOfDiceNode->execute(previous);
    auto lastnode= ParsingToolBox::getLeafNode(m_numberOfDiceNode);
    if(nullptr == lastnode)
        return;
    auto result= lastnode->getResult();
    if(nullptr == result)
        return;
    if(!result->hasResultOfType(Dice::RESULT_TYPE::SCALAR))
        return;

    auto numberOfDice= result->getResult(Dice::RESULT_TYPE::SCALAR).toInt();

    DiceResult* previousDiceResult= dynamic_cast<DiceResult*>(previous->getResult());
    m_result->setPrevious(previousDiceResult);

    if(isValid(!previousDiceResult, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid result")))
        return;

    QList<Die*> diceList= previousDiceResult->getResultList();

    if(numberOfDice < 0)
    {
        numberOfDice= diceList.size() + numberOfDice;
    }

    QList<Die*> diceList3= diceList.mid(0, static_cast<int>(numberOfDice));
    QList<Die*> diceList2;

    for(Die* die : std::as_const(diceList3))
    {
        Die* tmpdie= new Die(*die);
        diceList2.append(tmpdie);
        die->displayed();
        die->setSelected(false);
    }

    if(numberOfDice > static_cast<qint64>(diceList.size()))
    {
        m_errors.insert(
            Dice::ERROR_CODE::TOO_MANY_DICE,
            QObject::tr(" You ask to keep %1 dice but the result only has %2").arg(numberOfDice).arg(diceList.size()));
    }

    for(auto& tmp : diceList.mid(static_cast<int>(numberOfDice), -1))
    {
        tmp->setHighlighted(false);
    }

    m_diceResult->setResultList(diceList2);
}

void KeepDiceExecNode::setDiceKeepNumber(ExecutionNode* n)
{
    m_numberOfDiceNode= n;
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
    if(nullptr != m_numberOfDiceNode)
    {
        node->setDiceKeepNumber(m_numberOfDiceNode->getCopy());
    }
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
