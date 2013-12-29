#include "rerolldicenode.h"

RerollDiceNode::RerollDiceNode(ExecutionNode* previous)
    : m_previous(previous)
{

}
void RerollDiceNode::run()
{
    if((NULL!=m_previous)&&(NULL!=m_previous->getResult()))
    {
        QList<qint64> list = m_previous->getResult()->getResultList();


        for(qint64 i=0; i < list.size() ; ++i)
        {
           // m_result.insertResult(rollDice());
        }
        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
}

