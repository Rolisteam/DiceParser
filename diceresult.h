#ifndef DICERESULT_H
#define DICERESULT_H
#include <QList>

class DiceResult
{
public:
    DiceResult();

    qint64 getSum();
    QList<qint64>& getResultList();
    void insertResult(qint64);


private:
    QList<qint64> m_diceValues;
};

#endif // DICERESULT_H
