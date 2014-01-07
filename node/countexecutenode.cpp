#include "countexecutenode.h"
#include "diceresult.h"



CountExecuteNode::CountExecuteNode()
    : m_scalarResult(new ScalarResult())
{
    m_result = m_scalarResult;
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
    DiceResult* previous_result = static_cast<DiceResult*>(previous->getResult());
    if(NULL!=previous_result)
    {
        QList<Die> diceList=previous_result->getResultList();
        qint64 sum = 0;
        foreach(Die dice,diceList)
        {
            if(m_validator->isValid(dice))
            {
                ++sum;
            }
        }
        m_scalarResult->setValue(sum);


        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
}
