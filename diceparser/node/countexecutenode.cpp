#include "countexecutenode.h"
#include "result/diceresult.h"
#include "validatorlist.h"

CountExecuteNode::CountExecuteNode() : m_scalarResult(new ScalarResult()), m_validatorList(nullptr)
{
    m_result= m_scalarResult;
}
void CountExecuteNode::setValidatorList(ValidatorList* validatorlist)
{
    m_validatorList= validatorlist;
}
CountExecuteNode::~CountExecuteNode()
{
    if(nullptr != m_validatorList)
    {
        delete m_validatorList;
    }
}

void CountExecuteNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(nullptr == previous)
    {
        return;
    }
    DiceResult* previousResult= dynamic_cast<DiceResult*>(previous->getResult());
    if(nullptr != previousResult)
    {
        m_result->setPrevious(previousResult);
        qint64 sum= 0;
        std::function<void(Die*)> f= [&sum](const Die*) { sum+= 1; };
        m_validatorList->validResult(previousResult, true, true, f);
        m_scalarResult->setValue(sum);
        if(nullptr != m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
}
QString CountExecuteNode::toString(bool withlabel) const
{
    if(withlabel)
    {
        return QString("%1 [label=\"CountExecuteNode %2\"]").arg(m_id, m_validatorList->toString());
    }
    else
    {
        return m_id;
    }
}
qint64 CountExecuteNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}

ExecutionNode* CountExecuteNode::getCopy() const
{
    CountExecuteNode* node= new CountExecuteNode();
    if(nullptr != m_validatorList)
    {
        node->setValidatorList(m_validatorList->getCopy());
    }
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
