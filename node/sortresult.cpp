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
    if(NULL==node)
    {
        return;
    }
    DiceResult* previousDiceResult = static_cast<DiceResult*>(node->getResult());
    if(NULL!=previousDiceResult)
    {
        QList<Die> diceList=previousDiceResult->getResultList();
        QList<Die> diceList2=m_diceResult->getResultList();

        diceList2 = diceList;
        if(!m_ascending)
        {
            qSort(diceList2.begin(), diceList2.end(), qGreater<Die>());
        }
        else
        {
            qSort(diceList2.begin(), diceList2.end(), qLess<Die>());
        }
        m_diceResult->setResultList(diceList2);
        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }

}
void SortResultNode::setSortAscending(bool asc)
{
    m_ascending = asc;
}


bool operator<(Die const &a, Die const& b)
{
    if(a.getValue()<b.getValue())
        return true;
    else
        return false;
}
