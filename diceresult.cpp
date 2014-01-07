#include "diceresult.h"

DiceResult::DiceResult()
{

}
void DiceResult::insertResult(Die die)
{
    m_diceValues.append(die);
}
QList<Die>& DiceResult::getResultList()
{
    return m_diceValues;
}
void DiceResult::setResultList(QList<Die> list)
{
    m_diceValues.clear();
    m_diceValues << list;
}
bool DiceResult::isScalar() const
{
    if(m_diceValues.size()==1)
    {
        return true;
    }
    return false;
}
qint64 DiceResult::getScalar()
{

    if(m_diceValues.size()==1)
    {
        return m_diceValues[0].getValue();
    }
    return 0;
}
