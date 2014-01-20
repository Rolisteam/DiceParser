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
    if(NULL!=m_internalNode)
    {
        m_internalNode->run(this);
        ExecutionNode* temp=m_internalNode;
       while(NULL!=temp->getNextNode())
       {
            temp=temp->getNextNode();
       }
       m_result = m_internalNode->getResult();
    }


    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
