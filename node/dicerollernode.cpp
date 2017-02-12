#include "dicerollernode.h"
#include "die.h"


#include <QThread>
#include <QThreadPool>
#include <QDebug>
#include <QTime>



DiceRollerNode::DiceRollerNode(quint64 max,qint64 min)
    : m_max(max),m_diceResult(new DiceResult()),m_min(min)
{
	m_result=m_diceResult;
}
void DiceRollerNode::run(ExecutionNode* previous)
{
	m_previousNode = previous;
    if(NULL!=previous)
    {
        Result* result=previous->getResult();
        if(NULL!=result)
        {
            m_diceCount = result->getResult(Result::SCALAR).toReal();
            m_result->setPrevious(result);

            if(m_diceCount == 0)
            {
                m_errors.insert(NO_DICE_TO_ROLL,QObject::tr("No dice to roll"));
            }

            for(quint64 i=0; i < m_diceCount ; ++i)
            {
                Die* die = new Die();
                die->setBase(m_min);
                die->setMaxValue(m_max);
                die->roll();
                //qDebug() << die->getValue() << "value";
				m_diceResult->insertResult(die);
            }
            if(NULL!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
	}
}

quint64 DiceRollerNode::getFaces() const
{
    return abs(m_max-m_min)+1;
}
QString DiceRollerNode::toString(bool wl) const
{
	if(wl)
	{
        return QString("%1 [label=\"DiceRollerNode faces: %2\"]").arg(m_id).arg(getFaces());
	}
	else
	{
		return m_id;
	}
}
qint64 DiceRollerNode::getPriority() const
{
    qint64 priority=4;
//    if(NULL!=m_nextNode)
//    {
//        priority = m_nextNode->getPriority();
//    }


    return priority;
}
ExecutionNode* DiceRollerNode::getCopy() const
{
    DiceRollerNode* node = new DiceRollerNode(m_max,m_min);
    if(NULL!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
