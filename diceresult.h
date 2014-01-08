#ifndef DICERESULT_H
#define DICERESULT_H
#include <QList>

#include "die.h"
#include "result.h"

class DiceResult : public Result
{
public:
    DiceResult();

    qint64 getSum();
    QList<Die*>& getResultList();
    void insertResult(Die*);

    void setResultList(QList<Die*> list);

    bool isScalar() const;
    virtual qint64 getScalar();
private:
    QList<Die*> m_diceValues;
};

#endif // DICERESULT_H
