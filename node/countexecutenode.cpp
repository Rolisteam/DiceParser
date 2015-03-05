#include "countexecutenode.h"
#include "result/diceresult.h"



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
	m_previousNode = previous;
    if(NULL==previous)
    {
        return;
    }
    DiceResult* previous_result = dynamic_cast<DiceResult*>(previous->getResult());
    if(NULL!=previous_result)
    {
        m_result->setPrevious(previous_result);
        QList<Die*> diceList=previous_result->getResultList();
        qint64 sum = 0;
        foreach(Die* dice,diceList)
        {
            sum+=m_validator->hasValid(dice,true);
        }
        m_scalarResult->setValue(sum);


        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
}
QString CountExecuteNode::toString() const
{
    return "CountExecuteNode"+m_validator->toString();
}
qint64 CountExecuteNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
