#include "dicerollernode.h"
#include "die.h"


#include <QDateTime>
#include <QDebug>

DiceRollerNode::DiceRollerNode(quint64 faces)
    : m_faces(faces)
{
    uint seed = quintptr(this) + QDateTime::currentDateTime().toMSecsSinceEpoch();
    qsrand(seed);
}
void DiceRollerNode::run(ExecutionNode* previous)
{
    if(NULL!=previous)
    {
        m_diceCount = previous->getResult()->getSum();
        for(quint64 i=0; i < m_diceCount ; ++i)
        {
            Die die;
            die.setValue(rollDice());
            m_result.insertResult(die);
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
