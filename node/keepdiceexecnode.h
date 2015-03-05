#ifndef KEEPDICEEXECNODE_H
#define KEEPDICEEXECNODE_H

#include "executionnode.h"
#include "result/diceresult.h"

class KeepDiceExecNode : public ExecutionNode
{
public:
    KeepDiceExecNode();

    virtual void run(ExecutionNode *previous);
    virtual void setDiceKeepNumber(quint64 );
    virtual QString toString()const;
    virtual qint64 getPriority() const;
private:
    quint64 m_numberOfDice;
    DiceResult* m_diceResult;
};

#endif // KEEPDICEEXECNODE_H
