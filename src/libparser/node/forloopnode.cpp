#include "forloopnode.h"

#include "die.h"

MockNode::MockNode() {}

void MockNode::run(ExecutionNode* node)
{
    return;
}

void MockNode::setResult(Result* result)
{
    m_result= result;
}

QString MockNode::toString(bool) const
{
    return {};
};
qint64 MockNode::getPriority() const
{
    return 0;
}
ExecutionNode* MockNode::getCopy() const
{
    return new MockNode();
}
// end mocknode

ForLoopNode::ForLoopNode() : m_diceResult(new DiceResult) {}

void ForLoopNode::setInternal(ExecutionNode* node)
{
    m_internal.reset(node);
}

void ForLoopNode::run(ExecutionNode* previous)
{
    if(nullptr != previous)
    {
        auto prevResult= dynamic_cast<DiceResult*>(previous->getResult());
        if(nullptr != prevResult)
        {
            m_diceResult->setPrevious(prevResult);
            QList<Die*> diceList= prevResult->getResultList();
            for(Die* dice : diceList)
            {
                MockNode node;
                DiceResult diceResult;
                diceResult.insertResult(dice);
                node.setResult(&diceResult);
                m_internal->run(&node);

                auto tmp= m_internal.get();
                while(nullptr != tmp->getNextNode())
                {
                    tmp= tmp->getNextNode();
                }
                Result* internalResult= tmp->getResult();
                auto value= internalResult->getResult(Dice::RESULT_TYPE::SCALAR).toInt();

                Die* neodie= new Die();
                *neodie= *dice;
                neodie->setValue(value);
                m_diceResult->insertResult(neodie);
                node.setResult(nullptr);
                diceResult.clear();
                dice->displayed();
            }
        }
    }
    m_result= m_diceResult;
    if(m_nextNode != nullptr)
        m_nextNode->run(this);
}

qint64 ForLoopNode::getPriority() const
{
    return 2;
}

QString ForLoopNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return QString("%1 [label=\"ForLoopNode Node\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}

ExecutionNode* ForLoopNode::getCopy() const
{
    auto node= new ForLoopNode();
    if(m_internal)
    {
        node->setInternal(m_internal->getCopy());
    }
    return node;
}
