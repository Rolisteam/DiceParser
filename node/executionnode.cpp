#include "executionnode.h"

ExecutionNode::ExecutionNode()
    : m_nextNode(NULL),m_result(NULL),m_previousNode(NULL)
{

}
ExecutionNode::~ExecutionNode()
{

	if(NULL!=m_result)
	{
		delete m_result;
		m_result = NULL;
	}
	if(NULL!=m_nextNode)
	{
		delete m_nextNode;
		m_nextNode = NULL;
	}
}

Result* ExecutionNode::getResult()
{
    return m_result;
}
void ExecutionNode::setNextNode(ExecutionNode* node)
{
    m_nextNode = node;
}
ExecutionNode* ExecutionNode::getNextNode()
{
    return m_nextNode;
}
QList<ExecutionNode::ERROR_CODE> ExecutionNode::getErrorList()
{
    return m_errors;
}
QString ExecutionNode::getHelp()
{
    return QString();
}
ExecutionNode* ExecutionNode::getPreviousNode() const
{
	return m_previousNode;
}
void ExecutionNode::generateDotTree(QString& s)
{
    s.append(toString());
    if(NULL!=m_nextNode)
    {
        s.append(" -> ");
        s.append(m_nextNode->toString());
        s.append(" [label=\"nextNode\"];\n");
        m_nextNode->generateDotTree(s);
    }
    else
    {
        s.append(" -> ");
        s.append("NULL");
        s.append(" [label=\"nextNode\"];\n");

        m_result->generateDotTree(s);
    }
}
