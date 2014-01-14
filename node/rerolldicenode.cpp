#include "rerolldicenode.h"


RerollDiceNode::RerollDiceNode()
    : m_myDiceResult(new DiceResult())
{
    m_result=m_myDiceResult;
}
void RerollDiceNode::run(ExecutionNode* previous)
{
    if((NULL!=previous)&&(NULL!=previous->getResult()))
    {
        DiceResult* previous_result = static_cast<DiceResult*>(previous->getResult());
        m_result->setPrevious(previous_result);
        if(NULL!=previous_result)
        {
            QList<Die*> list = previous_result->getResultList();


            foreach(Die* die, list)
            {
                if(m_validator->isValid(die))
                {
                    die->roll();
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
