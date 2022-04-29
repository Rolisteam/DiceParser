/***************************************************************************
 *   Copyright (C) 2016 by Renaud Guezennec                                *
 *   https://rolisteam.org/contact                   *
 *                                                                         *
 *   rolisteam is free software; you can redistribute it and/or modify     *
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
#include "ifnode.h"
#include "result/diceresult.h"
#include "validatorlist.h"

PartialDiceRollNode::PartialDiceRollNode() : m_diceResult(new DiceResult)
{
    m_result= m_diceResult;
}

void PartialDiceRollNode::insertDie(Die* die)
{
    m_diceResult->insertResult(die);
}

void PartialDiceRollNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    Result* presult= previous->getResult();
    if(nullptr != presult)
    {
        m_result->setPrevious(presult);
    }
    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}
ExecutionNode* PartialDiceRollNode::getCopy() const
{
    return new PartialDiceRollNode();
}
qint64 PartialDiceRollNode::getPriority() const
{
    qint64 priority= 4;
    return priority;
}

QString PartialDiceRollNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return QString("%1 [label=\"PartialDiceRollNode \"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}

DiceResult* getFirstDiceResult(Result* result)
{
    DiceResult* found= nullptr;

    if(nullptr == result)
        return found;
    do
    {
        found= dynamic_cast<DiceResult*>(result);
        result= result->getPrevious();
    } while((nullptr == found) && (result != nullptr));

    return found;
}

IfNode::IfNode() : m_conditionType(Dice::AllOfThem), m_true(nullptr), m_false(nullptr)
{
    // m_result = new DiceResult();
}

IfNode::~IfNode()
{
    m_result= nullptr;
}

void IfNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(nullptr == previous)
    {
        return;
    }
    ExecutionNode* previousLoop= previous;
    ExecutionNode* nextNode= nullptr;
    bool runNext= (nullptr == m_nextNode) ? false : true;
    Result* previousResult= previous->getResult();
    m_result= previousResult->getCopy();

    if(nullptr != m_result)
    {
        qreal value= previousResult->getResult(Dice::RESULT_TYPE::SCALAR).toReal();

        if(nullptr != m_validatorList)
        {
            DiceResult* previousDiceResult= getFirstDiceResult(previousResult);
            if(nullptr != previousDiceResult)
            {
                QList<Die*> diceList= previousDiceResult->getResultList();

                if(m_conditionType == Dice::OnEach)
                {
                    for(Die* dice : diceList)
                    {
                        auto diceNode= new PartialDiceRollNode();
                        diceNode->insertDie(new Die(*dice));
                        if(m_validatorList->hasValid(dice, true, true))
                        {
                            nextNode= (nullptr == m_true) ? nullptr : m_true->getCopy();
                        }
                        else
                        {
                            nextNode= (nullptr == m_false) ? nullptr : m_false->getCopy();
                        }

                        if(nullptr != nextNode)
                        {
                            if(nullptr == previousLoop->getNextNode())
                            {
                                previousLoop->setNextNode(nextNode);
                            }
                            if(nullptr == m_nextNode)
                            {
                                m_nextNode= nextNode;
                            }
                            diceNode->setNextNode(nextNode);
                            diceNode->run(previousLoop);
                            previousLoop= getLeafNode(nextNode);
                        }
                    }
                }
                else if((m_conditionType == Dice::OneOfThem) || (m_conditionType == Dice::AllOfThem))
                {
                    bool trueForAll= true;
                    bool falseForAll= true;

                    bool oneIsTrue= false;
                    bool oneIsFalse= false;

                    for(Die* dice : diceList)
                    {
                        bool result= m_validatorList->hasValid(dice, true, true);
                        trueForAll= trueForAll ? result : false;
                        falseForAll= falseForAll ? result : false;

                        oneIsTrue|= result;
                        oneIsFalse= !result ? true : oneIsFalse;
                    }
                    if(m_conditionType == Dice::OneOfThem)
                    {
                        if(oneIsTrue)
                        {
                            nextNode= (nullptr == m_true) ? nullptr : m_true->getCopy();
                        }
                        else // if(oneIsFalse)
                        {
                            nextNode= (nullptr == m_false) ? nullptr : m_false->getCopy();
                        }
                    }
                    else if(m_conditionType == Dice::AllOfThem)
                    {
                        if(trueForAll)
                        {
                            nextNode= (nullptr == m_true) ? nullptr : m_true->getCopy();
                        }
                        else // if(falseForAll)
                        {
                            nextNode= (nullptr == m_false) ? nullptr : m_false->getCopy();
                        }
                    }

                    if(nullptr != nextNode)
                    {
                        if(nullptr == m_nextNode)
                        {
                            m_nextNode= nextNode;
                        }
                        nextNode->run(previousLoop);
                        previousLoop= getLeafNode(nextNode);
                    }
                }
            }

            if(m_conditionType == Dice::OnScalar)
            {
                Die dice;
                auto val= static_cast<qint64>(value);
                dice.setValue(val);
                dice.insertRollValue(val);
                dice.setMaxValue(val);
                if(m_validatorList->hasValid(&dice, true, true))
                {
                    nextNode= m_true;
                }
                else
                {
                    nextNode= m_false;
                }
                if(nullptr != nextNode)
                {
                    if(nullptr == m_nextNode)
                    {
                        m_nextNode= nextNode;
                    }
                    nextNode->run(previousLoop);
                    previousLoop= getLeafNode(nextNode);
                }
            }
        }
    }

    if((nullptr != m_nextNode) && (runNext))
    {
        m_nextNode->run(previousLoop);
    }
}

void IfNode::setValidatorList(ValidatorList* val)
{
    m_validatorList= val;
}
void IfNode::setInstructionTrue(ExecutionNode* node)
{
    m_true= node;
}

void IfNode::setInstructionFalse(ExecutionNode* node)
{
    m_false= node;
}
void IfNode::generateDotTree(QString& s)
{
    s.append(toString(true));
    s.append(";\n");

    if((nullptr != m_true) && (m_true != m_nextNode))
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_true->toString(false));
        s.append("[label=\"true" + m_validatorList->toString() + "\"];\n");

        m_true->generateDotTree(s);
    }
    if((nullptr != m_false) && (m_false != m_nextNode))
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_false->toString(false));
        s.append("[label=\"false\"];\n");
        m_false->generateDotTree(s);
    }

    if(nullptr != m_nextNode)
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_nextNode->toString(false));
        s.append("[label=\"next\"];\n");
        m_nextNode->generateDotTree(s);
    }
    else
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append("nullptr;\n");

        if(nullptr != m_result)
        {
            s.append(toString(false));
            s.append(" ->");
            s.append(m_result->toString(false));
            s.append(" [label=\"Result\"];\n");
            m_result->generateDotTree(s);
        }
    }
}

QString IfNode::toString(bool wl) const
{
    if(wl)
    {
        return QString("%1 [label=\"IfNode\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}

qint64 IfNode::getPriority() const
{
    return 0;
}

ExecutionNode* IfNode::getLeafNode(ExecutionNode* node)
{
    ExecutionNode* next= node;
    while(nullptr != next->getNextNode())
    {
        next= next->getNextNode();
    }
    return next;
}

Dice::ConditionType IfNode::getConditionType() const
{
    return m_conditionType;
}

void IfNode::setConditionType(const Dice::ConditionType& conditionType)
{
    m_conditionType= conditionType;
}
ExecutionNode* IfNode::getCopy() const
{
    IfNode* node= new IfNode();

    node->setConditionType(m_conditionType);
    if(nullptr != m_validatorList)
    {
        node->setValidatorList(m_validatorList->getCopy());
    }
    if(nullptr != m_false)
    {
        node->setInstructionFalse(m_false->getCopy());
    }
    if(nullptr != m_true)
    {
        node->setInstructionTrue(m_true->getCopy());
    }
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
