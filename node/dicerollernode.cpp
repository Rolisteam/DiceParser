#include "dicerollernode.h"
#include "die.h"


#include <QThread>
#include <QThreadPool>
#include <QDebug>
#include <QTime>


//DiceRoller::DiceRoller(QMutex* mutex,DiceResult* diceResult,int faces,int count)
//    : m_mutex(mutex),m_sharedDiceResult(diceResult),m_faces(faces),m_diceCount(count)
//{

//}

//void DiceRoller::run()
//{
//    for(quint64 i=0; i < m_diceCount ; ++i)
//    {
//        Die* die = new Die();
//        die->setFaces(m_faces);
//        die->roll();
//        m_mutex->lock();
//        m_sharedDiceResult->insertResult(die);
//        m_mutex->unlock();
//    }
//}



//////////////////////////////////////////////////
/// \brief DiceRollerNode::DiceRollerNode
//////////////////////////////////////////////////
DiceRollerNode::DiceRollerNode(quint64 faces,qint64 offset)
    : m_faces(faces),m_diceResult(new DiceResult()),m_offset(offset)
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

            for(quint64 i=0; i < m_diceCount ; ++i)
            {
                Die* die = new Die();
                die->setFaces(m_faces);
                die->setBase(m_offset);
                die->roll();
				m_diceResult->insertResult(die);
            }
            if(NULL!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
    }



}

quint64 DiceRollerNode::getFaces()
{
    return m_faces;
}
QString DiceRollerNode::toString(bool wl) const
{
	if(wl)
	{
		return QString("%1 [label=DiceRollerNode]").arg(m_id);
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
