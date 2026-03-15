/***************************************************************************
 * Copyright (C) 2014 by Renaud Guezennec                                   *
 * https://rolisteam.org/contact                      *
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
#include "groupnode.h"
#include "result/diceresult.h"
//-------------------------------
int DieGroup::getSum() const
{
    int sum= 0;
    for(int i : *this)
    {
        sum+= i;
    }
    return sum;
}

void DieGroup::removeValue(DieGroup i)
{
    for(auto x : i)
    {
        removeOne(x);
    }
}

void DieGroup::sort()
{
    std::sort(std::begin(*this), std::end(*this), std::greater<qint64>());
}

int DieGroup::getLost() const
{
    return getSum() - m_exceptedValue;
}

qint64 DieGroup::getExceptedValue() const
{
    return m_exceptedValue;
}

void DieGroup::setExceptedValue(qint64 exceptedValue)
{
    m_exceptedValue= exceptedValue;
}

//---------------------
GroupNode::GroupNode(bool complexOutput)
    : ExecutionNode("%1 [label=\"SplitNode Node\"]")
    , m_scalarResult(new ScalarResult)
    , m_stringResult(new StringResult)
    , m_complexOutput(complexOutput)
{
}

void GroupNode::run(ExecutionNode* previous)
{
    if(m_complexOutput)
        m_result= m_stringResult;
    else
        m_result= m_scalarResult;

    m_previousNode= previous;
    if(isValid(!m_previousNode, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No Previous node")))
        return;

    if(!m_result)
        return;

    m_result->setPrevious(previous->getResult());
    Result* tmpResult= previous->getResult();

    if(isValid(!tmpResult, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid result")))
        return;

    DiceResult* dice= dynamic_cast<DiceResult*>(tmpResult);

    if(isValid(!dice, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid Dice result")))
        return;

    auto allDicelist= dice->getResultList();
    DieGroup allResult;
    for(auto& die : allDicelist)
    {
        allResult << die->getListValue();
    }
    std::sort(allResult.begin(), allResult.end(), std::greater<qint64>());
    if(allResult.getSum() <= m_groupValue)
    {
        m_scalarResult->setValue(0);
        return;
    }

    auto copy= allResult;
    auto const die= getGroup(allResult);

    for(auto& list : die)
    {
        for(auto& val : list)
        {
            copy.removeOne(val);
        }
    }
    m_scalarResult->setValue(die.size());
    QStringList list;
    for(auto group : die)
    {
        QStringList values;
        std::transform(group.begin(), group.end(), std::back_inserter(values),
                       [](qint64 val) { return QString::number(val); });
        list << QStringLiteral("{%1}").arg(values.join(","));
    }
    QStringList unused;
    std::transform(copy.begin(), copy.end(), std::back_inserter(unused),
                   [](qint64 val) { return QString::number(val); });
    if(!unused.isEmpty())
        m_stringResult->addText(
            QStringLiteral("%1 (%2 - [%3])").arg(die.size()).arg(list.join(",")).arg(unused.join(",")));
    else
        m_stringResult->addText(QStringLiteral("%1 (%2)").arg(die.size()).arg(list.join(",")));
}

qint64 GroupNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_nextNode)
    {
        priority= m_nextNode->getPriority();
    }
    return priority;
}
ExecutionNode* GroupNode::getCopy() const
{
    GroupNode* node= new GroupNode(m_complexOutput);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

int GroupNode::getGroupValue() const
{
    return m_groupValue;
}

void GroupNode::setGroupValue(qint64 groupValue)
{
    m_groupValue= groupValue;
}

bool GroupNode::composeWithPrevious(DieGroup previous, qint64 first, qint64 current, DieGroup& addValue)
{
    if(previous.getSum() + first + current == m_groupValue)
    {
        addValue.append(previous);
        addValue.append(first);
        addValue.append(current);
        return true;
    }

    if(previous.isEmpty())
        return false;

    int maxComboLength= previous.size();
    bool hasReachMax= false;

    QList<DieGroup> possibleUnion;
    for(auto va : previous)
    {
        DieGroup dieG;
        dieG.append(va);
        possibleUnion.append(dieG);
    }

    while(!hasReachMax)
    {
        auto tmpValues= previous;
        QList<DieGroup> possibleTmp;
        for(auto& diaG : possibleUnion)
        {
            if(tmpValues.isEmpty())
                break;
            tmpValues.removeValue(diaG);

            for(auto& value : tmpValues)
            {
                DieGroup dia;
                dia.append(diaG);
                dia.append(value);
                if(dia.size() >= maxComboLength - 1)
                    hasReachMax= true;
                else
                    possibleTmp.append(dia);
            }
        }
        if(possibleTmp.isEmpty())
            hasReachMax= true;
        else
        {
            possibleTmp.append(possibleUnion);
            possibleUnion= possibleTmp;
        }
    }
    std::sort(possibleUnion.begin(), possibleUnion.end(),
              [=](const DieGroup& a, const DieGroup& b) { return a.getLost() > b.getLost(); });
    bool found= false;
    for(int i= 0; (!found && i < possibleUnion.size()); ++i)
    {
        auto& value= possibleUnion.at(i);
        if(value.getSum() + current + first >= m_groupValue)
        {
            addValue << value << current << first;
            found= true;
        }
    }
    return found;
}

DieGroup GroupNode::findPerfect(DieGroup values, int count, int currentValue) const
{
    for(auto it= values.rbegin(); it != values.rend(); ++it)
    {
        DieGroup g;
        for(int i= 0; i < count && (it + i) != values.rend(); ++i)
            g << *(it + i);

        if(currentValue + g.getSum() == m_groupValue)
            return g;
    }
    return {};
}

DieGroup GroupNode::findCombo(DieGroup values, int count, int currentValue) const
{
    for(auto it= values.rbegin(); it != values.rend(); ++it)
    {
        DieGroup g;
        for(int i= 0; i < count && (it + i) != values.rend(); ++i)
            g << *(it + i);

        if(currentValue + g.getSum() > m_groupValue)
            return g;
    }
    return {};
}

QList<DieGroup> GroupNode::getGroup(DieGroup values)
{
    if(values.isEmpty())
        return {};

    bool bigger= true;
    QMap<qint64, DieGroup> loseMap;
    QList<DieGroup> result;
    do
    {
        auto it= std::begin(values);
        bigger= (*it >= m_groupValue);
        if(!bigger)
            continue;

        DieGroup group;
        group << *it;
        result << group;
        values.remove(0);

    } while(bigger && !values.isEmpty());

    auto remaining= values;
    while(remaining.getSum() >= m_groupValue)
    {
        if(remaining.isEmpty())
            break;

        QList<qint64> useless;
        bool perfect= false;
        // find perfect
        do
        {
            auto base= remaining[0];
            remaining.removeFirst();
            perfect= false;
            for(int i= 1; i <= remaining.size() && !perfect; ++i)
            {
                auto foundValues= findPerfect(remaining, i, base);
                perfect= !foundValues.isEmpty();
                if(perfect)
                {
                    remaining.removeValue(foundValues);
                    foundValues.prepend(base);
                    foundValues.sort();
                    result.append(foundValues);
                }
            }

            if(!perfect)
                useless.append(base);

        } while(!remaining.isEmpty());

        remaining << useless;

        // find group with lost
        bool found= false;
        if(remaining.isEmpty())
            break;
        do
        {
            auto base= remaining[0];
            remaining.removeFirst();
            found= false;
            for(int i= 1; i <= remaining.size() && !found; ++i)
            {

                auto foundValues= findCombo(remaining, i, base);

                found= !foundValues.isEmpty();
                if(found)
                {
                    remaining.removeValue(foundValues);
                    foundValues.append(base);
                    foundValues.sort();
                    foundValues.setExceptedValue(m_groupValue);

                    loseMap[foundValues.getLost()]= foundValues;
                    result.append(foundValues);
                }
            }
        } while(found && !remaining.isEmpty());
    }

    return result;
}
