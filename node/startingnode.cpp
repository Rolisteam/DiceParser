#include "startingnode.h"
#include <QDebug>

StartingNode::StartingNode()
{

}
void StartingNode::run(ExecutionNode*)
{
    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
