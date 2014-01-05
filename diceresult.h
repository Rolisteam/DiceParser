#ifndef DICERESULT_H
#define DICERESULT_H
#include <QList>

#include "die.h"

class DiceResult
{
public:
    DiceResult();

    qint64 getSum();
    QList<Die>& getResultList();
    void insertResult(Die);

    void setResultList(QList<Die> list);


private:
    QList<Die> m_diceValues;
};

#endif // DICERESULT_H
