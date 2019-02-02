#include "dicerollernode.h"
#include "die.h"

#include <QDebug>
#include <QThread>
#include <QThreadPool>
#include <QTime>

DiceRollerNode::DiceRollerNode(qint64 max, qint64 min)
    : m_max(max), m_diceResult(new DiceResult()), m_min(min), m_operator(Die::PLUS)
{
    m_result= m_diceResult;
}
void DiceRollerNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(nullptr != previous)
    {
        Result* result= previous->getResult();
        if(nullptr != result)
        {
            m_diceCount= static_cast<quint64>(result->getResult(Result::SCALAR).toReal());
            m_result->setPrevious(result);

            if(m_diceCount == 0)
            {
                m_errors.insert(NO_DICE_TO_ROLL, QObject::tr("No dice to roll"));
            }
            auto possibleValue= static_cast<quint64>(std::abs((m_max - m_min) + 1));
            if(possibleValue < m_diceCount && m_unique)
            {
                m_errors.insert(
                    TOO_MANY_DICE, QObject::tr("More unique values asked than possible values (D operator)"));
                return;
            }

            for(quint64 i= 0; i < m_diceCount; ++i)
            {
                Die* die= new Die();
                die->setOp(m_operator);
                die->setBase(m_min);
                die->setMaxValue(m_max);
                die->roll();
                if(m_unique)
                {
                    const auto& equal= [](const Die* a, const Die* b) { return a->getValue() == b->getValue(); };
                    while(m_diceResult->contains(die, equal))
                    {
                        die->roll(false);
                    }
                }
                m_diceResult->insertResult(die);
            }
            if(nullptr != m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
    }
}

quint64 DiceRollerNode::getFaces() const
{
    return std::abs(m_max - m_min) + 1;
}

std::pair<qint64, qint64> DiceRollerNode::getRange() const
{
    return std::make_pair(m_min, m_max);
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
    qint64 priority= 4;
    //    if(nullptr!=m_nextNode)
    //    {
    //        priority = m_nextNode->getPriority();
    //    }
    return priority;
}
ExecutionNode* DiceRollerNode::getCopy() const
{
    DiceRollerNode* node= new DiceRollerNode(m_max, m_min);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

Die::ArithmeticOperator DiceRollerNode::getOperator() const
{
    return m_operator;
}

void DiceRollerNode::setOperator(const Die::ArithmeticOperator& dieOperator)
{
    m_operator= dieOperator;
    m_diceResult->setOperator(dieOperator);
}

bool DiceRollerNode::getUnique() const
{
    return m_unique;
}

void DiceRollerNode::setUnique(bool unique)
{
    m_unique= unique;
}
