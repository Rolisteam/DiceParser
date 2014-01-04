#include "keepdiceexecnode.h"

KeepDiceExecNode::KeepDiceExecNode()
{
}

void KeepDiceExecNode::run(ExecutionNode* previous)
{
    if(NULL==previous)
    {
        return;
    }
    QList<qint64> diceList=previous->getResult()->getResultList();
    QList<qint64> diceList2=m_result.getResultList();


    diceList2 = diceList.mid(0,m_numberOfDice);
    m_result.setResultList(diceList2);
    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
void KeepDiceExecNode::setDiceKeepNumber(quint64 n)
{
    m_numberOfDice = n;
}
