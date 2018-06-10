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
	if(nullptr!=m_diceResult)
	{
		delete m_diceResult;
		m_diceResult =nullptr;
	}
}

QStringList ListSetRollNode::getList() const
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
    if(nullptr!=previous)
    {
        Result* result=previous->getResult();
        if(nullptr!=result)
        {
            quint64 diceCount = result->getResult(Result::SCALAR).toReal();
            if(diceCount > static_cast<quint64>(m_values.size()) && m_unique)
            {
                m_errors.insert(TOO_MANY_DICE,QObject::tr("More unique values asked than possible values (L operator)"));
            }
            else
            {
                m_result->setPrevious(result);
                QStringList rollResult;
                for(quint64 i=0; i < diceCount ; ++i)
                {
                    Die* die = new Die(1,computeFacesNumber());
                    die->roll();
                    m_diceResult->insertResult(die);
                    getValueFromDie(die,rollResult);
                }
                m_stringResult->setText(rollResult.join(","));
            }
            if(nullptr!=m_nextNode)
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
qint64 ListSetRollNode::computeFacesNumber()
{
    if(m_rangeList.isEmpty())
    {
        return m_values.size();
    }
    else
    {
        Q_ASSERT(m_values.size() == m_rangeList.size());
        qint64 max;
        int i=0;
        for(Range range: m_rangeList)
        {
            if(((i==0)||(max<range.getEnd()))&&(range.isFullyDefined()))
            {
                max= range.getEnd();
            }
            ++i;
        }
        return max;
    }

}
void ListSetRollNode::getValueFromDie(Die* die,QStringList& rollResult)
{
    if(m_rangeList.isEmpty())
    {
        if(die->getValue()-1<m_values.size())
        {
            auto str = m_values[die->getValue()-1];
            while(m_unique && rollResult.contains(str))
            {
                die->roll(false);
                str = m_values[die->getValue()-1];
            }
            rollResult << str;
        }
    }
    else
    {
        Q_ASSERT(m_values.size() == m_rangeList.size());
        bool found = false;
        while(!found)
        {
            int i=0;
            for (Range& range:  m_rangeList)
            {
                auto it = std::find(m_rangeIndexResult.begin(),m_rangeIndexResult.end(),i);
                auto isValid = range.hasValid(die,false);
                if((isValid && !m_unique)||
                        (isValid && it == m_rangeIndexResult.end()))
                {
                    m_rangeIndexResult.push_back(i);
                    rollResult << m_values[i];
                    found=true;
                }
                ++i;
            }
            if(!found)
            {
                die->roll(false);
            }
        }
    }
}
ExecutionNode* ListSetRollNode::getCopy() const
{
    ListSetRollNode* node = new ListSetRollNode();
    QList<Range> dataList = m_rangeList;
    node->setRangeList(dataList);
    node->setUnique(m_unique);
    node->setListValue(m_values);
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;

}
