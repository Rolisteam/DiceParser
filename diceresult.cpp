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
