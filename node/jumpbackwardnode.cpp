#include "jumpbackwardnode.h"


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
                if((i==0)&&(parent->getResult()->hasResultOfType(Result::DICE_LIST)))
				{
					found =true;
				}
			}
			parent = parent->getPreviousNode();

		}
        DiceResult* diceResult = dynamic_cast<DiceResult*>(result);
        foreach(Die* die,diceResult->getResultList())
        {
            Die* tmpdie = new Die();
            *tmpdie=*die;
            m_diceResult->insertResult(tmpdie);
        }

        //*m_result = *result;
        m_result->setPrevious(parent->getResult());

        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
}
