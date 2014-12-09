#include "executionnode.h"

ExecutionNode::ExecutionNode()
    : m_nextNode(NULL),m_result(NULL)
{

}
ExecutionNode::~ExecutionNode()
{

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
