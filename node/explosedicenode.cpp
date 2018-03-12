#include "explosedicenode.h"

ExploseDiceNode::ExploseDiceNode()
	: m_diceResult(new DiceResult()),m_validator(nullptr)
{
    m_result = m_diceResult;
}
void ExploseDiceNode::run(ExecutionNode* previous)
{
	m_previousNode = previous;
    if((nullptr!=previous)&&(nullptr!=previous->getResult()))
    {
        DiceResult* previous_result = dynamic_cast<DiceResult*>(previous->getResult());
        m_result->setPrevious(previous_result);
        if(nullptr!=previous_result)
        {
            for(Die* die: previous_result->getResultList())
            {
                Die* tmpdie = new Die();
                *tmpdie=*die;
                m_diceResult->insertResult(tmpdie);
                die->displayed();
            }

            QList<Die*> list = m_diceResult->getResultList();


            for(Die* die: list)
            {
                while(m_validator->hasValid(die,false))
                {
                    die->roll(true);
                }
            }
           // m_diceResult->setResultList(list);

            if(nullptr!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
        else
        {
            qDebug() << "test!!";
        }
    }
}
ExploseDiceNode::~ExploseDiceNode()
{
	if(nullptr!=m_validator)
	{
		delete m_validator;
	}
}
void ExploseDiceNode::setValidator(Validator* val)
{
      m_validator = val;
}
QString ExploseDiceNode::toString(bool withlabel) const
{
	if(withlabel)
	{
		return QString("%1 [label=\"ExploseDiceNode %2\"]").arg(m_id).arg(m_validator->toString());
	}
	else
	{
		return m_id;
	}
}
qint64 ExploseDiceNode::getPriority() const
{
    qint64 priority=0;
    if(nullptr!=m_previousNode)
    {
        priority = m_previousNode->getPriority();
    }
    return priority;
}

ExecutionNode* ExploseDiceNode::getCopy() const
{
    ExploseDiceNode* node = new ExploseDiceNode();
    if(nullptr!=m_validator)
    {
        node->setValidator(m_validator->getCopy());
    }
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
