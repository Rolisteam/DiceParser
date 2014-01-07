#include "rerolldicenode.h"
#include "dicerollernode.h"

RerollDiceNode::RerollDiceNode()
{

}
void RerollDiceNode::run(ExecutionNode* previous)
{
    if((NULL!=previous)&&(NULL!=previous->getResult()))
    {
        DiceResult* previous_result = static_cast<DiceResult*>(previous->getResult());
        if(NULL!=previous_result)
        {
            QList<Die> list = previous_result->getResultList();


            foreach(Die die, list)
            {
                if(m_value == die.getValue())
                {
    /*
                    DiceRollerNode roller;
                    roller.run(this);*/
                }
            }

            if(NULL!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
    }
}
