#include "diceresult.h"

DiceResult::DiceResult()
{

}
void DiceResult::insertResult(qint64 die)
{
    m_diceValues.append(die);
}
QList<qint64>& DiceResult::getResultList()
{
    return m_diceValues;
}
qint64  DiceResult::getSum()
{
    qint64 sum=0;
    foreach (qint64 tmp, m_diceValues)
    {
        sum+=tmp;
    }
    return sum;
}

