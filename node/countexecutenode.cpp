#include "countexecutenode.h"
#include "result/diceresult.h"



CountExecuteNode::CountExecuteNode()
	: m_scalarResult(new ScalarResult()),m_validator(NULL)
{
	m_result = m_scalarResult;
}
void CountExecuteNode::setValidator(Validator* validator)
{
	m_validator = validator;
}
CountExecuteNode::~CountExecuteNode()
{
	if(NULL!=m_validator)
	{
		delete m_validator;
	}
}

void CountExecuteNode::run(ExecutionNode *previous)
{
	m_previousNode = previous;
	if(NULL==previous)
	{
		return;
	}
    DiceResult* previousResult = dynamic_cast<DiceResult*>(previous->getResult());
    if(NULL!=previousResult)
	{
        m_result->setPrevious(previousResult);
        QList<Die*> diceList=previousResult->getResultList();
		qint64 sum = 0;
		foreach(Die* dice,diceList)
		{
            if(NULL!=m_validator)
            {
                sum+=m_validator->hasValid(dice,true,true);
            }
		}
		m_scalarResult->setValue(sum);


		if(NULL!=m_nextNode)
		{
			m_nextNode->run(this);
		}
	}
}
QString CountExecuteNode::toString(bool withlabel) const
{
	if(withlabel)
	{
		return QString("%1 [label=\"CountExecuteNode %2\"]").arg(m_id).arg(m_validator->toString());
	}
	else
	{
		return m_id;
	}
}
qint64 CountExecuteNode::getPriority() const
{
	qint64 priority=0;
	if(NULL!=m_nextNode)
	{
		priority = m_nextNode->getPriority();
	}


    return priority;
}

ExecutionNode* CountExecuteNode::getCopy() const
{
    CountExecuteNode* node = new CountExecuteNode();
    if(NULL!=m_validator)
    {
        node->setValidator(m_validator->getCopy());
    }
    if(NULL!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;

}
