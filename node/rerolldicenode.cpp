#include "rerolldicenode.h"
#include "dicerollernode.h"

RerollDiceNode::RerollDiceNode()
{

}
void RerollDiceNode::run(ExecutionNode* previous)
{
    if((NULL!=previous)&&(NULL!=previous->getResult()))
    {
        QList<Die> list = previous->getResult()->getResultList();


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

