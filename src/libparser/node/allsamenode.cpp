#include "allsamenode.h"

AllSameNode::AllSameNode() : ExecutionNode("%1 [label=\"AllSameNode\"]"), m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}

void AllSameNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(isValid(!m_previousNode, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No valid previous node")))
        return;

    DiceResult* previous_result= dynamic_cast<DiceResult*>(previous->getResult());
    if(isValid(!previous_result, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No dice result")))
        return;

    m_result->setPrevious(previous_result);
    bool allSame= true;
    int i= 0;
    qint64 previousValue= 0;
    if(isValid(previous_result->getResultList().size() < 2, Dice::ERROR_CODE::ENDLESS_LOOP_ERROR,
               QStringLiteral("T operator must operate on more than 1 die")))
        return;

    for(auto& die : previous_result->getResultList())
    {
        if(i == 0)
            previousValue= die->getValue();
        Die* tmpdie= new Die(*die);
        m_diceResult->insertResult(tmpdie);
        die->displayed();
        if(previousValue != die->getValue())
            allSame= false;
        ++i;
    }

    while(allSame)
    {
        QList<Die*> list= m_diceResult->getResultList();
        qint64 pValue= 0;
        int i= 0;
        for(auto& die : list)
        {
            die->roll(true);
            if(i == 0)
                pValue= die->getValue();
            if(pValue != die->getValue())
                allSame= false;
            ++i;
        }
    }
}

qint64 AllSameNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_nextNode)
    {
        priority= m_nextNode->getPriority();
    }
    return priority;
}

ExecutionNode* AllSameNode::getCopy() const
{
    return new AllSameNode();
}
