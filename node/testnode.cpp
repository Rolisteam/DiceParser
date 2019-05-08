#include "testnode.h"
#include "die.h"

TestNode::TestNode() {}

TestNode::~TestNode()
{
    m_nextNode= nullptr;
    m_result= nullptr;
}
void TestNode::run(ExecutionNode* previous)
{
    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}

QString TestNode::toString(bool wl) const
{
    if(wl)
    {
        return QStringLiteral("%1 [label=\"TestNode \"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}
qint64 TestNode::getPriority() const
{
    qint64 priority= 4;
    return priority;
}
ExecutionNode* TestNode::getCopy() const
{
    TestNode* node= new TestNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

void TestNode::setResult(Result* result)
{
    m_result= result;
}
