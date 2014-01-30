#include "startingnode.h"
#include <QDebug>

StartingNode::StartingNode()
{

}
void StartingNode::run(ExecutionNode*)
{
//    qDebug() << "starting node";
    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString StartingNode::toString() const
{
    return "Starting node";
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
