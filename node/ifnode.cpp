/***************************************************************************
    *   Copyright (C) 2016 by Renaud Guezennec                                *
    *   http://www.rolisteam.org/contact                   *
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

IfNode::IfNode()
    : m_validator(NULL),m_true(NULL),m_false(NULL),m_conditionType(AllOfThem)
{
    //m_result = new DiceResult();
}

IfNode::~IfNode()
{
    m_result=NULL;
}

void IfNode::run(ExecutionNode *previous)
{
    m_previousNode = previous;
    if(NULL==previous)
    {
        return;
    }
    ExecutionNode* previousLoop = previous;
    ExecutionNode* nextNode = NULL;
    bool runNext = (NULL==m_nextNode) ? false : true;
    Result* previousResult = previous->getResult();
    m_result = previousResult;

    if(NULL!=m_result)
    {
        qreal value = previousResult->getResult(Result::SCALAR).toReal();

        if(NULL!=m_validator)
        {
            DiceResult* previousDiceResult = dynamic_cast<DiceResult*>(previousResult);
            if(NULL!=previousDiceResult)
            {
                QList<Die*> diceList=previousDiceResult->getResultList();
                if(m_conditionType == OnEach)
                {
                    for(Die* dice : diceList)
                    {
                        if(m_validator->hasValid(dice,true,true))
                        {
                            nextNode = (NULL==m_true) ? NULL: m_true->getCopy();
                        }
                        else
                        {
                            nextNode = (NULL==m_false) ? NULL: m_false->getCopy();
                        }
                        if(NULL!=nextNode)
                        {
                            if(NULL==previousLoop->getNextNode())
                            {
                                previousLoop->setNextNode(nextNode);
                            }
                            if(NULL==m_nextNode)
                            {
                                m_nextNode = nextNode;
                            }
                            nextNode->run(previousLoop);
                            previousLoop = getLeafNode(nextNode);
                        }
                    }
                }
                else
                {
                    bool trueForAll=true;
                    bool falseForAll=true;

                    bool oneIsTrue=false;
                    bool oneIsFalse=false;

                    for(Die* dice : diceList)
                    {
                        bool result = m_validator->hasValid(dice,true,true);
                        trueForAll = trueForAll ? result : false;
                        falseForAll = falseForAll ? result : false;

                        oneIsTrue = (oneIsTrue==false) ? result : true;
                        oneIsFalse = (oneIsFalse==false) ? result : true;
                    }
                    if(m_conditionType==OneOfThem)
                    {
                        if(oneIsTrue)
                        {
                            nextNode = (NULL==m_true) ? NULL: m_true->getCopy();
                        }
                        else if(oneIsFalse)
                        {
                             nextNode = (NULL==m_false) ? NULL: m_false->getCopy();
                        }
                    }
                    else if(m_conditionType==AllOfThem)
                    {
                        if(trueForAll)
                        {
                            nextNode = (NULL==m_true) ? NULL: m_true->getCopy();
                        }
                        else if(falseForAll)
                        {
                             nextNode = (NULL==m_false) ? NULL: m_false->getCopy();
                        }
                    }


                    if(NULL!=nextNode)
                    {
                        if(NULL==m_nextNode)
                        {
                            m_nextNode = nextNode;
                        }
                        nextNode->run(previousLoop);
                        previousLoop = getLeafNode(nextNode);
                    }
                }
            }
            else
            {
                Die* dice = new Die();
                dice->setValue(value);
                dice->setMaxValue(value);
                if(m_validator->hasValid(dice,true,true))
                {
                        nextNode=m_true;
                }
                else
                {
                        nextNode=m_false;
                }
                if(NULL!=nextNode)
                {
                    if(NULL==m_nextNode)
                    {
                        m_nextNode = nextNode;
                    }
                    nextNode->run(previousLoop);
                    previousLoop = getLeafNode(nextNode);
                }
            }
        }
    }

    if((NULL!=m_nextNode)&&(runNext))
    {
        m_nextNode->run(previousLoop);
    }
}

void IfNode::setValidator(Validator* val)
{
       m_validator = val;
}
void IfNode::setInstructionTrue(ExecutionNode* node)
{
    m_true = node;
}

void IfNode::setInstructionFalse(ExecutionNode* node)
{
    m_false = node;
}
void IfNode::generateDotTree(QString& s)
{
    s.append(toString(true));
    s.append(";\n");

    if((NULL!=m_true)&&(m_true != m_nextNode))
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_true->toString(false));
        s.append("[label=\"true\"];\n");
//        s.append(" [label=\"nextNode\"];\n");
        m_true->generateDotTree(s);
    }
    if((NULL!=m_false)&&(m_false != m_nextNode))
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_false->toString(false));
        s.append("[label=\"false\"];\n");
        m_false->generateDotTree(s);
    }

    if(NULL!=m_nextNode)
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
        s.append("NULL;\n");

        if(NULL!=m_result)
        {

            s.append(toString(false));
            s.append(" ->");
            s.append(m_result->toString(false));
            s.append(" [label=\"Result\"];\n");
            m_result->generateDotTree(s);
        }
    }
}

QString IfNode::toString(bool wl)  const
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
    return 4;
}

ExecutionNode* IfNode::getLeafNode(ExecutionNode* node)
{
    ExecutionNode* next = node;
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
    }
    return next;
}

IfNode::ConditionType IfNode::getConditionType() const
{
    return m_conditionType;
}

void IfNode::setConditionType(const IfNode::ConditionType &conditionType)
{
    m_conditionType = conditionType;
}
ExecutionNode* IfNode::getCopy() const
{
    IfNode* node = new IfNode();
    if(NULL!=m_validator)
    {
        node->setValidator(m_validator->getCopy());
    }
    if(NULL!=m_false)
    {
        node->setInstructionFalse(m_false->getCopy());
    }
    if(NULL!=m_true)
    {
        node->setInstructionTrue(m_true->getCopy());
    }
    if(NULL!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;

}
