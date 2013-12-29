#include "executionnode.h"

ExecutionNode::ExecutionNode()
    : m_nextNode(NULL)
{

}
ExecutionNode::~ExecutionNode()
{

}

DiceResult* ExecutionNode::getResult()
{
    return &m_result;
}
void ExecutionNode::setNextNode(ExecutionNode* node)
{
    m_nextNode = node;
}
ExecutionNode* ExecutionNode::getNextNode()
{
    return m_nextNode;
}
