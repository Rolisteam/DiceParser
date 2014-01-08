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
        QList<Die*> diceList=previousDiceResult->getResultList();
        QList<Die*> diceList2=m_diceResult->getResultList();

        /*diceList2 = diceList;

        if(!m_ascending)
        {
            qSort(diceList2.begin(), diceList2.end(), qGreater<Die*>());
        }
        else
        {
            qSort(diceList2.begin(), diceList2.end(), qLess<Die*>());
        }*/


        for(int i = 0; i<diceList.size();++i)
        {
            Die* tmp1 = diceList[i];
            int j =0;
            bool found = false;
            for(; ((j < diceList2.size())&&(!found)); ++j)
            {
                Die* tmp2 = diceList2[j];
//                qDebug() << tmp1->getValue() << tmp2->getValue()  << j;
                if(tmp1->getValue() < tmp2->getValue())
                {
                    found = true;
                }
            }
            if(found)
                diceList2.insert(j-1,tmp1);
            else
                diceList2.append(tmp1);

        }

        if(!m_ascending)
        {
            for(int i = 0; i< diceList2.size()/2; ++i)
            {
                diceList2.swap(i,diceList2.size()-(1+i));
            }

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
