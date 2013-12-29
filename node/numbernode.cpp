#include "numbernode.h"

NumberNode::NumberNode()
{

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
    m_result.insertResult(a);
    m_number = a;
}
