#include "numbernode.h"

NumberNode::NumberNode()
    : m_scalarResult(new ScalarResult())
{
    m_result = m_scalarResult;
}
void NumberNode::run(ExecutionNode* previous)
{
    if(NULL!=previous)
    {
        m_result->setPrevious(previous->getResult());
    }
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
QString NumberNode::toString() const
{
    return QString("NumberNode:%1").arg(m_number);
}
qint64 NumberNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
