/***************************************************************************
    *   Copyright (C) 2016 by Renaud Guezennec                                *
    *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
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
{
    //m_result = new DiceResult();
}

IfNode::~IfNode()
{

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
    Result* previousResult = previous->getResult();
    m_result = previousResult;
    DiceResult* previousDiceResult = dynamic_cast<DiceResult*>(previousResult);

    if(NULL!=previousDiceResult)
    {
        qreal value = previousResult->getResult(Result::SCALAR).toReal();

        QList<Die*> diceList=previousDiceResult->getResultList();


        if(NULL!=m_validator)
        {
            if(!diceList.isEmpty())
            {
                foreach(Die* dice,diceList)
                {
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
            else
            {
                Die* dice = new Die();
                dice->setValue(value);
                dice->setFaces(value);
                m_validator->hasValid(dice,true,true);
            }
        }
    }

   /* if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }*/
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

QString IfNode::toString(bool) const
{

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
