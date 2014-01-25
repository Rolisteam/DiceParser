#include "dicerollernode.h"
#include "die.h"



#include <QDebug>

DiceRollerNode::DiceRollerNode(quint64 faces)
    : m_faces(faces),m_myDiceResult(new DiceResult())
{

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
            m_diceCount = result->getScalar();
            m_result->setPrevious(result);

            for(quint64 i=0; i < m_diceCount ; ++i)
            {
                Die* die = new Die();
                die->setFaces(m_faces);
                die->roll();
                m_myDiceResult->insertResult(die);
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
QString DiceRollerNode::toString() const
{
    return QString("DiceRollerNode");
}
