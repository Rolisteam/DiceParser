#include "countexecutenode.h"

CountExecuteNode::CountExecuteNode()
{
}
void CountExecuteNode::setValidator(Validator* validator)
{
    m_validator = validator;
}

void CountExecuteNode::run(ExecutionNode *previous)
{
    if(NULL==previous)
    {
        return;
    }
    QList<qint64> diceList=previous->getResult()->getResultList();
   qint64 sum = 0;
    foreach(qint64 dice,diceList)
    {
        if(m_validator->isValid(dice))
        {
            ++sum;
        }
    }
    m_result.insertResult(sum);


    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
