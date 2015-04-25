#include "jumpbackwardnode.h"
#include <QDebug>

JumpBackwardNode::JumpBackwardNode()
{
    m_previousNode=NULL;
    m_diceResult =new DiceResult();
    m_result = m_diceResult;
}



qint64 JumpBackwardNode::getPriority() const
{
    return 4;
}
QString JumpBackwardNode::toString() const
{
    return QString("JumpBackwardNode");
}
void JumpBackwardNode::run(ExecutionNode* previous)
{
        m_previousNode = previous;
		ExecutionNode* parent = previous;
		bool found=false;
        int i = 3;
		Result* result=NULL;
        while((NULL!=parent)&&(!found))
		{

			result = parent->getResult();
            if((NULL!=result))
			{
				--i;
                if((i==0)&&(result->hasResultOfType(Result::DICE_LIST)))
				{
					found =true;
				}
			}
            if(!found)
            {
                parent = parent->getPreviousNode();
            }

		}
        DiceResult* diceResult = dynamic_cast<DiceResult*>(result);
        foreach(Die* die,diceResult->getResultList())
        {
            Die* tmpdie = new Die();
            *tmpdie=*die;
            m_diceResult->insertResult(tmpdie);
            die->displayed();
        }


        m_result->setPrevious(parent->getResult());



        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
        for(int i =0;i<diceResult->getResultList().size();++i)
        {
            Die* tmp =diceResult->getResultList().at(i);
            Die* tmp2 =m_diceResult->getResultList().at(i);
            if(tmp->isHighlighted())
            {
                tmp2->setHighlighted(true);
            }
        }


}
