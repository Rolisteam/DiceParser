#include "stringnode.h"

StringNode::StringNode()
    : m_stringResult(new StringResult())
{
    m_result = m_stringResult;
}

void StringNode::run(ExecutionNode *previous)
{
    m_previousNode = previous;
    if(nullptr!=previous)
    {
        m_result->setPrevious(previous->getResult());
    }
    if(nullptr!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}

void StringNode::setString(QString str)
{
    m_data = str;
    m_stringResult->setText(m_data);
}
QString StringNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        QString dataCopy = m_data;

        return QString("%1 [label=\"StringNode %2\"]").arg(m_id).arg(dataCopy.replace('%','\\'));
    }
    else
    {
        return m_id;
    }
}
qint64 StringNode::getPriority() const
{
    qint64 priority=0;
    if(nullptr!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }
    return priority;
}
ExecutionNode* StringNode::getCopy() const
{
    StringNode* node = new StringNode();
    node->setString(m_data);
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;

}
