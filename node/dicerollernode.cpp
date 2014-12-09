#include "dicerollernode.h"
#include "die.h"


#include <QThread>
#include <QThreadPool>
#include <QDebug>
#include <QTime>


DiceRoller::DiceRoller(QMutex* mutex,DiceResult* diceResult,int faces,int count)
    : m_mutex(mutex),m_sharedDiceResult(diceResult),m_faces(faces),m_diceCount(count)
{

}

void DiceRoller::run()
{
    for(quint64 i=0; i < m_diceCount ; ++i)
    {
        Die* die = new Die();
        die->setFaces(m_faces);
        die->roll();
        m_mutex->lock();
        m_sharedDiceResult->insertResult(die);
        m_mutex->unlock();
    }
}



//////////////////////////////////////////////////
/// \brief DiceRollerNode::DiceRollerNode
//////////////////////////////////////////////////
DiceRollerNode::DiceRollerNode(quint64 faces)
    : m_faces(faces),m_myDiceResult(new DiceResult())
{
    m_mutex=new QMutex();
    m_result=m_myDiceResult;

}
void DiceRollerNode::run(ExecutionNode* previous)
{
//       qDebug() << "DiceRollerNode node";
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
                die->roll();
                m_myDiceResult->insertResult(die);
            }

           /* quint64 threadCount = QThread::idealThreadCount();
            if(threadCount>m_diceCount)
            {
                threadCount=m_diceCount;
            }
            quint64 dicePass = m_diceCount/threadCount;
            quint64 remainingDiceCount=m_diceCount;

            QThreadPool threadpool;


            for(int i=threadCount-1;i>=0 && remainingDiceCount>0;--i)
            {
                remainingDiceCount-=dicePass;
                if((remainingDiceCount<dicePass)||((i==0)&&(remainingDiceCount!=0)))
                {
                    dicePass+=remainingDiceCount;
                    remainingDiceCount=0;
                }
                qDebug() << remainingDiceCount << dicePass << i << threadCount;
                threadpool.start(new DiceRoller(m_mutex,m_myDiceResult,m_faces,dicePass));
            }

            threadpool.waitForDone();*/
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
QString DiceRollerNode::toString() const
{
    return QString("DiceRollerNode");
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
