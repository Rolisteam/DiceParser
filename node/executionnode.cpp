#include "executionnode.h"

#include <QUuid>

ExecutionNode::ExecutionNode()
    : m_previousNode(NULL),m_result(NULL),m_nextNode(NULL),m_errors(QMap<ExecutionNode::DICE_ERROR_CODE,QString>()),m_id(QString("\"%1\"").arg(QUuid::createUuid().toString()))
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
QMap<ExecutionNode::DICE_ERROR_CODE,QString> ExecutionNode::getExecutionErrorMap()
{
    if(NULL!=m_nextNode)
    {
        foreach (ExecutionNode::DICE_ERROR_CODE key, m_nextNode->getExecutionErrorMap().keys())
        {
            m_errors.insert(key,m_nextNode->getExecutionErrorMap().value(key));
        }
    }
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
	s.append(toString(true));
	s.append(";\n");

    if(NULL!=m_nextNode)
    {
		s.append(toString(false));
        s.append(" -> ");
		s.append(m_nextNode->toString(false));
        s.append("[label=\"next\"];\n");
//        s.append(" [label=\"nextNode\"];\n");
        m_nextNode->generateDotTree(s);
    }
    else
    {
		s.append(toString(false));
        s.append(" -> ");
		s.append("NULL;\n");
        if(NULL!=m_result)
        {

            s.append(toString(false));
            s.append(" ->");
            s.append(m_result->toString(false));
            s.append(" [label=\"Result\"];\n");


            m_result->generateDotTree(s);
        }
    }

}
