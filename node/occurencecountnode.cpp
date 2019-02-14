#include "occurencecountnode.h"
#include "result/diceresult.h"
#include "result/stringresult.h"

OccurenceCountNode::OccurenceCountNode() : ExecutionNode()
{
    m_stringResult= new StringResult();
    m_result= m_stringResult;
}

void OccurenceCountNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    std::map<qint64, qint64> mapOccurence;
    if(nullptr == m_previousNode)
        return;

    DiceResult* previousDiceResult= dynamic_cast<DiceResult*>(m_previousNode->getResult());
    // m_diceResult->setPrevious(previousDiceResult);
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

    QStringList list;
    for(auto key : mapOccurence)
    {
        if(key.first >= m_height && key.second >= m_width)
            list << QStringLiteral("%1x%2").arg(key.first).arg(key.second);
    }

    QStringList resultList;
    std::for_each(vec.begin(), vec.end(), [&resultList](qint64 val) { resultList << QString::number(val); });

    QString result;

    if(!list.isEmpty())
        result= list.join(',');
    else
        result= QObject::tr("No matching result");

    m_stringResult->setText(QStringLiteral("%1 - [%2]").arg(result).arg(resultList.join(',')));

    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
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
