#include "allsamenode.h"


AllSameNode::AllSameNode() : m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}

void AllSameNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(nullptr != previous)
    {
        DiceResult* previous_result= dynamic_cast<DiceResult*>(previous->getResult());
		if(nullptr != previous_result)
        {
        	m_result->setPrevious(previous_result);
			bool allSame=true;
			int i=0;
			quint64 previousValue;
			for(auto& die : previous_result->getResultList())
            {   
				if(i == 0)
					previousValue=die->getValue();
                Die* tmpdie= new Die(*die);
                m_diceResult->insertResult(tmpdie);
                die->displayed();
				if(previousValue != die->getValue())
					allSame=false;
				++i;
            }

			while(allSame)
			{
				QList<Die*> list= m_diceResult->getResultList();
				qint64 pValue;
				int i =0;
				for(auto& die: list)
				{
					die->roll(true);
					if(i==0)
						pValue=die->getValue();
					if(pValue != die->getValue())
						allSame = false;
					++i;
				}
			}
		}
    }
    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}


QString AllSameNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return QString("%1 [label=\"AllSameNode\"]").arg(m_id);
    }
    else
    {
        return m_id;
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
