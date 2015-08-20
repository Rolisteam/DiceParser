#include "startingnode.h"
#include <QDebug>

StartingNode::StartingNode()
{

}
void StartingNode::run(ExecutionNode*)
{
	m_previousNode = NULL;
    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString StartingNode::toString(bool withlabel) const
{
if(withlabel)
    return "StartingNode [shape=box]";
else
    return "StartingNode";
}

}
qint64 StartingNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
