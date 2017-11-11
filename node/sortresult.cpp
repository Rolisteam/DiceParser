/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
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
#include "sortresult.h"

#include <QDebug>
#include "die.h"

SortResultNode::SortResultNode()
    : m_diceResult(new DiceResult)
{
    m_ascending = true;
    m_result = m_diceResult;

}
void SortResultNode::run(ExecutionNode* node)
{
	m_previousNode = node;
    if(nullptr==node)
    {
        return;
    }
    DiceResult* previousDiceResult = dynamic_cast<DiceResult*>(node->getResult());
    m_diceResult->setPrevious(previousDiceResult);
    if(nullptr!=previousDiceResult)
    {   
        QList<Die*> diceList=previousDiceResult->getResultList();
        QList<Die*> diceList2=m_diceResult->getResultList();

        // half-interval search sorting
        for(int i = 0; i<diceList.size();++i)
        {

            Die* tmp1 = new Die();
            *tmp1=*diceList[i];
            diceList[i]->displayed();

            int j =0;
            bool found = false;
            int start = 0;
            int end = diceList2.size();
            Die* tmp2 = nullptr;
            while(!found)
            {
                int distance = end-start;
                j = (start+end)/2;
                if(distance == 0)
                {
                    j=end;
                    found=true;
                }
                else
                {
                    tmp2 = diceList2[j];
                    if(tmp1->getValue() < tmp2->getValue())
                    {
                            end=j;
                    }
                    else
                    {
                            start=j+1;
                    }
                }
            }
            diceList2.insert(j,tmp1);
        }

        if(!m_ascending)
        {
            for(int i = 0; i< diceList2.size()/2; ++i)
            {
                diceList2.swap(i,diceList2.size()-(1+i));
            }

        }
        m_diceResult->setResultList(diceList2);
        if(nullptr!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
    else
    {
        //m_result = node->getResult();
        //m_errors.append(DIE_RESULT_EXPECTED);
    }

}
void SortResultNode::setSortAscending(bool asc)
{
    m_ascending = asc;
}
QString SortResultNode::toString(bool wl) const
{
	if(wl)
	{
		return QString("%1 [label=\"SortResultNode %2\"]").arg(m_id).arg(m_ascending ? "Ascending":"Descending");
	}
	else
	{
		return m_id;
	}

}
qint64 SortResultNode::getPriority() const
{
    qint64 priority=0;
    if(nullptr!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
ExecutionNode* SortResultNode::getCopy() const
{
    SortResultNode* node = new SortResultNode();
    node->setSortAscending(m_ascending);
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;

}
