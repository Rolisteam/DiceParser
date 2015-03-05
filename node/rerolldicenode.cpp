#include "rerolldicenode.h"


RerollDiceNode::RerollDiceNode()
    : m_myDiceResult(new DiceResult()),m_adding(false)
{
    m_result=m_myDiceResult;
}
void RerollDiceNode::run(ExecutionNode* previous)
{
    m_previousNode = previous;
    if((NULL!=previous)&&(NULL!=previous->getResult()))
    {
        DiceResult* previous_result = static_cast<DiceResult*>(previous->getResult());
        m_result->setPrevious(previous_result);
        if(NULL!=previous_result)
        {
            QList<Die*> list = previous_result->getResultList();


            foreach(Die* die, list)
            {
                if(m_validator->hasValid(die,false))
                {
                    die->roll(m_adding);
                }
            }
            m_myDiceResult->setResultList(list);

            if(NULL!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
    }
}
void RerollDiceNode::setValidator(Validator* val)
{
      m_validator = val;
}
QString RerollDiceNode::toString() const
{
    return "RerollDiceNode";
}
void RerollDiceNode::setAddingMode(bool b)
{
    m_adding = b;
}
qint64 RerollDiceNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
