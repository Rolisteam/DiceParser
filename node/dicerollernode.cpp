#include "dicerollernode.h"
#include "die.h"


#include <QDateTime>
#include <QDebug>

DiceRollerNode::DiceRollerNode(quint64 faces)
    : m_faces(faces),m_myDiceResult(new DiceResult())
{
    uint seed = quintptr(this) + QDateTime::currentDateTime().toMSecsSinceEpoch();
    qsrand(seed);
    m_result=m_myDiceResult;

}
void DiceRollerNode::run(ExecutionNode* previous)
{
    if(NULL!=previous)
    {
        m_diceCount = previous->getResult()->getScalar();
        for(quint64 i=0; i < m_diceCount ; ++i)
        {
            Die die;
            die.insertRollValue(rollDice());
            m_myDiceResult->insertResult(die);
        }
        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
}
quint64 DiceRollerNode::rollDice()
{
    return (qrand()%m_faces)+1;
}
