#include <QList>


#include "keepdiceexecnode.h"


KeepDiceExecNode::KeepDiceExecNode()
    : m_diceResult(new DiceResult())
{
    m_result = m_diceResult;
}

void KeepDiceExecNode::run(ExecutionNode* previous)
{
    if(NULL==previous)
    {
        return;
    }
    DiceResult* previousDiceResult = static_cast<DiceResult*>(previous->getResult());
    if(NULL!=previousDiceResult)
    {
        QList<Die> diceList=previousDiceResult->getResultList();
        QList<Die> diceList2=m_diceResult->getResultList();


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
