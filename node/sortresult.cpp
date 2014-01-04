#include "sortresult.h"

#include <QDebug>

SortResultNode::SortResultNode()
{
    m_ascending = true;
}
void SortResultNode::run(ExecutionNode* node)
{
    if(NULL==node)
    {
        return;
    }
    QList<qint64> diceList=node->getResult()->getResultList();
    QList<qint64> diceList2=m_result.getResultList();

    diceList2 = diceList;
    if(!m_ascending)
    {
        qSort(diceList2.begin(), diceList2.end(), qGreater<int>());
    }
    else
    {
        qSort(diceList2.begin(), diceList2.end(), qLess<int>());
    }
    m_result.setResultList(diceList2);
    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }

}
void SortResultNode::setSortAscending(bool asc)
{
    m_ascending = asc;
}
