#include "rerolldicenode.h"


RerollDiceNode::RerollDiceNode()
    : m_diceResult(new DiceResult()),m_adding(false),m_validator(nullptr)
{
    m_result=m_diceResult;
}
RerollDiceNode::~RerollDiceNode()
{
	if(nullptr!=m_validator)
	{
		delete m_validator;
		m_validator = nullptr;
	}
}
void RerollDiceNode::run(ExecutionNode* previous)
{
    m_previousNode = previous;
    if((nullptr!=previous)&&(nullptr!=previous->getResult()))
    {
        DiceResult* previous_result = static_cast<DiceResult*>(previous->getResult());
        m_result->setPrevious(previous_result);
        if(nullptr!=previous_result)
        {
            foreach(Die* die,previous_result->getResultList())
            {
                Die* tmpdie = new Die();
                *tmpdie=*die;
                m_diceResult->insertResult(tmpdie);
                die->displayed();
            }
            //m_diceResult->setResultList(list);

            QList<Die*> list = m_diceResult->getResultList();

            foreach(Die* die, list)
            {
                if(m_validator->hasValid(die,false))
                {
                    die->roll(m_adding);
                }
            }

            if(nullptr!=m_nextNode)
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
QString RerollDiceNode::toString(bool wl) const
{
	if(wl)
	{
		return QString("%1 [label=\"RerollDiceNode validatior: %2\"]").arg(m_id).arg(m_validator->toString());
	}
	else
	{
		return m_id;
	}
	//return QString("RerollDiceNode [label=\"RerollDiceNode validatior:%1\"");
}
void RerollDiceNode::setAddingMode(bool b)
{
    m_adding = b;
}
qint64 RerollDiceNode::getPriority() const
{
    qint64 priority=0;
    if(nullptr!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
ExecutionNode* RerollDiceNode::getCopy() const
{
    RerollDiceNode* node = new RerollDiceNode();
    node->setValidator(m_validator);
    node->setAddingMode(m_adding);
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
