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
#include "jumpbackwardnode.h"
#include <QDebug>

JumpBackwardNode::JumpBackwardNode()
{
    m_previousNode= nullptr;
    m_backwardNode= nullptr;
    m_diceResult= new DiceResult();
    m_result= m_diceResult;
}

qint64 JumpBackwardNode::getPriority() const
{
    return 4;
}
QString JumpBackwardNode::toString(bool wl) const
{
    if(wl)
    {
        return QString("%1 [label=\"JumpBackwardNode\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}
void JumpBackwardNode::generateDotTree(QString& s)
{
    s.append(toString(true));
    s.append(";\n");

    if(nullptr != m_backwardNode)
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_backwardNode->toString(false));
        s.append("[label=\"backward\"];\n");
        // m_backwardNode->generateDotTree(s);
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

void JumpBackwardNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    ExecutionNode* parent= previous;
    bool found= false;
    // int i = 3;
    Result* result= nullptr;
    while((nullptr != parent) && (!found))
    {
        result= parent->getResult();
        if(nullptr != result)
        {
            //--i;
            if(/*(i==0)&&*/ (result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST)))
            {
                found= true;
                m_backwardNode= parent;
            }
            else
            {
                JumpBackwardNode* jpNode= dynamic_cast<JumpBackwardNode*>(parent);
                if(nullptr != jpNode)
                {
                    found= true;
                    m_backwardNode= parent;
                }
            }
        }
        if(!found)
        {
            parent= parent->getPreviousNode();
        }
    }
    if(nullptr == result)
    {
        m_errors.insert(
            Dice::ERROR_CODE::DIE_RESULT_EXPECTED,
            QObject::tr(" The @ operator expects dice result. Please check the documentation to fix your command."));
    }
    else
    {
        DiceResult* diceResult= dynamic_cast<DiceResult*>(result);
        if(nullptr != diceResult)
        {
            for(auto& die : diceResult->getResultList())
            {
                Die* tmpdie= new Die(*die);
                //*tmpdie= *die;
                m_diceResult->insertResult(tmpdie);
                die->displayed();
            }
        }

        m_result->setPrevious(previous->getResult());

        if(nullptr != m_nextNode)
        {
            m_nextNode->run(this);
        }
        if(nullptr != diceResult)
        {
            for(int i= 0; i < diceResult->getResultList().size(); ++i)
            {
                Die* tmp= diceResult->getResultList().at(i);
                Die* tmp2= m_diceResult->getResultList().at(i);
                if(tmp->isHighlighted())
                {
                    tmp2->setHighlighted(true);
                }
            }
        }
    }
}

ExecutionNode* JumpBackwardNode::getCopy() const
{
    JumpBackwardNode* node= new JumpBackwardNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
