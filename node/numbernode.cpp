#include "numbernode.h"

NumberNode::NumberNode()
    : m_scalarResult(new ScalarResult())
{
    m_result = m_scalarResult;
}
void NumberNode::run(ExecutionNode* )
{
    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}

void NumberNode::setNumber(qint64 a)
{
    m_scalarResult->setValue(a);
    m_number = a;
}
