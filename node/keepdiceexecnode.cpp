#include <QList>


#include "keepdiceexecnode.h"


KeepDiceExecNode::KeepDiceExecNode()
    : m_diceResult(new DiceResult())
{
    m_result = m_diceResult;
}

void KeepDiceExecNode::run(ExecutionNode* previous)
{
m_previousNode = previous;
    if(NULL==previous)
    {
        return;
    }
    DiceResult* previousDiceResult = static_cast<DiceResult*>(previous->getResult());
    m_result->setPrevious(previousDiceResult);
    if(NULL!=previousDiceResult)
    {
        QList<Die*> diceList=previousDiceResult->getResultList();
        QList<Die*> diceList2=m_diceResult->getResultList();


        diceList2 = diceList.mid(0,m_numberOfDice);
        m_diceResult->setResultList(diceList2);
        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
}
void KeepDiceExecNode::setDiceKeepNumber(quint64 n)
{
    m_numberOfDice = n;
}
QString KeepDiceExecNode::toString() const
{
    return QString("KeepDiceExecNode");
}
qint64 KeepDiceExecNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
