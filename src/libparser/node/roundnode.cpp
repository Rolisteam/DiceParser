#include "roundnode.h"

#include <diceparser/parsingtoolbox.h>

RoundNode::RoundNode(Mode mode)
    : ExecutionNode("%1 [label=\"RoundNode\"]"), m_scalarResult(new ScalarResult), m_mode(mode)
{
}

void RoundNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;

    if(m_cmd == nullptr)
        return;

    m_cmd->execute(this);
    auto internal= ParsingToolBox::getLeafNode(m_cmd);
    if(!internal)
        return;

    auto endResult= internal->getResult();

    auto scalar= endResult->getResult(Dice::RESULT_TYPE::SCALAR).toDouble();

    int resVal;
    switch(m_mode)
    {
    case FLOOR:
        resVal= std::floor(scalar);
        break;
    case CEIL:
        resVal= std::ceil(scalar);
        break;
    case ROUND:
        resVal= std::round(scalar);
        break;
    }

    m_scalarResult->setValue(resVal);
    m_result= m_scalarResult.get();
}

qint64 RoundNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_nextNode)
    {
        priority= m_nextNode->getPriority();
    }

    return priority;
}

ExecutionNode* RoundNode::getCopy() const
{
    RoundNode* node= new RoundNode(m_mode);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

void RoundNode::setCommand(ExecutionNode* cmd)
{
    m_cmd= cmd;
}
