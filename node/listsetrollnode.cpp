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
#include "listsetrollnode.h"
#include "die.h"

ListSetRollNode::ListSetRollNode()
    :m_diceResult(new DiceResult()),m_stringResult(new StringResult()),m_unique(false)
{
    m_result = m_stringResult;
}
ListSetRollNode::~ListSetRollNode()
{
	if(NULL!=m_diceResult)
	{
		delete m_diceResult;
		m_diceResult =NULL;
	}
}

QStringList ListSetRollNode::getList()
{
    return m_values;
}
QString ListSetRollNode::toString(bool wl) const
{
	if(wl)
	{
		return QString("%1 [label=\"ListSetRoll list:%2\"]").arg(m_id).arg(m_values.join(","));
	}
	else
	{
		return m_id;
	}

}
qint64 ListSetRollNode::getPriority() const
{
    qint64 priority=4;
    return priority;
}
void ListSetRollNode::run(ExecutionNode* previous)
{
    m_previousNode = previous;
    if(NULL!=previous)
    {
        Result* result=previous->getResult();
        if(NULL!=result)
        {
            quint64 diceCount = result->getResult(Result::SCALAR).toReal();
            m_result->setPrevious(result);
            QStringList rollResult;
            for(quint64 i=0; i < diceCount ; ++i)
            {
                Die* die = new Die();
                computeFacesNumber(die);
                die->roll();
                m_diceResult->insertResult(die);
                getValueFromDie(die,rollResult);
            }
            m_stringResult->setText(rollResult.join(","));
            if(NULL!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
    }
}
void ListSetRollNode::setListValue(QStringList lirs)
{
    m_values = lirs;
}
void ListSetRollNode::setUnique(bool u)
{
    m_unique = u;
}
void ListSetRollNode::setRangeList(QList<Range>& ranges)
{
    m_rangeList = ranges;
}
void ListSetRollNode::computeFacesNumber(Die* die)
{
    if(m_rangeList.isEmpty())
    {
        die->setFaces(m_values.size());
    }
    else
    {
        Q_ASSERT(m_values.size() == m_rangeList.size());
        qint64 max;
        int i=0;
        foreach(Range range, m_rangeList)
        {
            if(((i==0)||(max<range.getEnd()))&&(range.isFullyDefined()))
            {
               // qDebug()<< range.isFullyDefined() << range.getEnd();
                max= range.getEnd();
            }
            ++i;
        }
        //qDebug() << "set Faces"<<max;
        die->setFaces(max);
    }

}
void ListSetRollNode::getValueFromDie(Die* die,QStringList& rollResult)
{
    if(m_rangeList.isEmpty())
    {
        if(die->getValue()-1<m_values.size())
        {
            rollResult << m_values[die->getValue()-1];
        }
    }
    else
    {
        Q_ASSERT(m_values.size() == m_rangeList.size());
        int i=0;
        foreach (Range range, m_rangeList)
        {
            if(range.hasValid(die,false))
            {
                rollResult << m_values[i];
            }
            ++i;
        }
    }
}
