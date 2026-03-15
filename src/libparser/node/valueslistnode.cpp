#include "valueslistnode.h"

#include "variablenode.h"

ValuesListNode::ValuesListNode() : ExecutionNode("%1 [label=\"ValuesListNode list\"]"), m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}

void ValuesListNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    for(auto node : m_data)
    {
        node->execute(this);
        auto result= node->getResult();
        if(!result)
            continue;
        auto val= result->getResult(Dice::RESULT_TYPE::SCALAR).toInt();
        Die* die= new Die();
        auto dyna= dynamic_cast<VariableNode*>(node);
        if(nullptr != dyna)
            dyna->setDisplayed();
        die->insertRollValue(val);
        m_diceResult->insertResult(die);
    }
}

void ValuesListNode::insertValue(ExecutionNode* value)
{
    m_data.push_back(value);
}
ExecutionNode* ValuesListNode::getCopy() const
{
    ValuesListNode* node= new ValuesListNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

qint64 ValuesListNode::getPriority() const
{
    qint64 priority= 4;
    return priority;
}
