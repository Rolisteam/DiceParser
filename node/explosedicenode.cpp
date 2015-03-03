#include "explosedicenode.h"

ExploseDiceNode::ExploseDiceNode()
    : m_diceResult(new DiceResult())
{
    m_result = m_diceResult;
}
void ExploseDiceNode::run(ExecutionNode* previous)
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
                while(m_validator->hasValid(die,false))
                {
                    die->roll(true);
                }
            }
            m_diceResult->setResultList(list);

            if(NULL!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
    }
}
void ExploseDiceNode::setValidator(Validator* val)
{
      m_validator = val;
}
QString ExploseDiceNode::toString() const
{
    return QString("ExploseDiceNode");
}
qint64 ExploseDiceNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
