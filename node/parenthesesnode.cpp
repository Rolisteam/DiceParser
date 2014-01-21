#ifndef PARENTHESESNODE_H
#define PARENTHESESNODE_H

#include "executionnode.h"
#include "parenthesesnode.h"

ParenthesesNode::ParenthesesNode()
{

}
void ParenthesesNode::setInternelNode(ExecutionNode* node)
{
    m_internalNode = node;
}
void ParenthesesNode::run(ExecutionNode* /*previous*/)
{
    qDebug() << "ParenthesesNode node";
    if(NULL!=m_internalNode)
    {
        m_internalNode->run(this);
        ExecutionNode* temp=m_internalNode;
       while(NULL!=temp->getNextNode())
       {
            temp=temp->getNextNode();
       }
       m_result = temp->getResult();
    }


    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString ParenthesesNode::toString() const
{
    return "ParenthesesNode";
}
