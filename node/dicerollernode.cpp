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
    if(NULL!=previous)
    {
        m_diceCount = previous->getResult()->getScalar();
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

quint64 DiceRollerNode::getFaces()
{
    return m_faces;
}
