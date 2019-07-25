/***************************************************************************
 *   Copyright (C) 2018 by Renaud Guezennec                                *
 *   http://www.rolisteam.org/contact                                      *
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
#include "occurencecountnode.h"
#include "result/diceresult.h"
#include "result/stringresult.h"
#include <QVector>

OccurenceCountNode::OccurenceCountNode() : ExecutionNode() {}

void OccurenceCountNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    std::map<qint64, qint64> mapOccurence;
    if(nullptr == m_previousNode)
        return;

    DiceResult* previousDiceResult= dynamic_cast<DiceResult*>(m_previousNode->getResult());
    if(nullptr == previousDiceResult)
        return;

    auto const& diceList= previousDiceResult->getResultList();
    QVector<qint64> vec;

    for(auto dice : diceList)
    {
        auto val= dice->getValue();

        vec << val;
        auto it= mapOccurence.find(val);
        if(it == mapOccurence.end())
            mapOccurence[val]= 1;
        else
            mapOccurence[val]+= 1;
    }

    std::sort(vec.begin(), vec.end());
    if(nullptr == m_nextNode)
    {
        runForStringResult(mapOccurence, vec);
    }
    else
    {
        runForDiceResult(mapOccurence);
    }
}
QString OccurenceCountNode::toString(bool label) const
{
    if(label)
    {
        return QString("%1 [label=\"OccurenceCountNode %2\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}
ExecutionNode* OccurenceCountNode::getCopy() const
{
    return nullptr;
}
qint64 OccurenceCountNode::getPriority() const
{
    qint64 priority= 0;

    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}

qint64 OccurenceCountNode::getWidth() const
{
    return m_width;
}

void OccurenceCountNode::setWidth(const qint64& width)
{
    m_width= width;
}

qint64 OccurenceCountNode::getHeight() const
{
    return m_height;
}

void OccurenceCountNode::setHeight(const qint64& height)
{
    m_height= height;
}

Validator* OccurenceCountNode::getValidator() const
{
    return m_validator;
}

void OccurenceCountNode::setValidator(Validator* validator)
{
    m_validator= validator;
}
void OccurenceCountNode::runForStringResult(const std::map<qint64, qint64>& mapOccurence, QVector<qint64>& vec)
{
    m_stringResult= new StringResult();
    m_result= m_stringResult;
    QStringList list;
    for(auto key : mapOccurence)
    {
        if(nullptr != m_validator)
        {
            Die die;
            die.insertRollValue(key.first);
            if(!m_validator->hasValid(&die, true))
                continue;
        }

        if(key.second < m_width)
            continue;

        if(key.first >= m_height)
            list << QStringLiteral("%1x%2").arg(key.second).arg(key.first);
    }

    QStringList resultList;
    std::for_each(vec.begin(), vec.end(), [&resultList](qint64 val) { resultList << QString::number(val); });

    QString result;

    if(!list.isEmpty())
        result= list.join(',');
    else
        result= QObject::tr("No matching result");

    m_stringResult->setText(QStringLiteral("%1 - [%2]").arg(result).arg(resultList.join(',')));
}
void OccurenceCountNode::runForDiceResult(const std::map<qint64, qint64>& mapOccurence)
{
    m_diceResult= new DiceResult();
    m_result= m_diceResult;
    QStringList list;
    for(auto key : mapOccurence)
    {
        if(nullptr != m_validator)
        {
            Die die;
            die.insertRollValue(key.first);
            if(!m_validator->hasValid(&die, true))
                continue;
        }

        if(key.second < m_width)
            continue;

        if(key.first >= m_height)
        {
            // list << QStringLiteral("%1x%2").arg(key.second).arg(key.first);
            Die* die= new Die();
            die->insertRollValue(key.second * key.first);
            m_diceResult->insertResult(die);
        }
    }

    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}
