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
	m_previousNode = node;
    if(NULL==node)
    {
        return;
    }
    DiceResult* previousDiceResult = dynamic_cast<DiceResult*>(node->getResult());
    m_diceResult->setPrevious(previousDiceResult);
    if(NULL!=previousDiceResult)
    {
        QList<Die*> diceList=previousDiceResult->getResultList();
        QList<Die*> diceList2=m_diceResult->getResultList();


        // Dichotomic sorting
        for(int i = 0; i<diceList.size();++i)
        {
            Die* tmp1 = diceList[i];
            int j =0;
            bool found = false;
            int start = 0;
            int end = diceList2.size();
            int distance = 0;
            Die* tmp2 = NULL;
            while(!found)
            {
                distance = end-start;
                j = (start+end)/2;


                if(distance == 0)
                {
                    j=end;
                    found=true;
                }
                else
                {
                    tmp2 = diceList2[j];
                    if(tmp1->getValue() < tmp2->getValue())
                    {
                            end=j;
                    }
                    else
                    {
                            start=j+1;
                    }

                }

            }
            diceList2.insert(j,tmp1);
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
    else
    {
            m_errors.append(DIE_RESULT_EXPECTED);
    }

}
void SortResultNode::setSortAscending(bool asc)
{
    m_ascending = asc;
}
QString SortResultNode::toString() const
{
    return "SortResultNode";
}
qint64 SortResultNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
