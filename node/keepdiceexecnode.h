#ifndef KEEPDICEEXECNODE_H
#define KEEPDICEEXECNODE_H

#include "executionnode.h"
#include "diceresult.h"

class KeepDiceExecNode : public ExecutionNode
{
public:
    KeepDiceExecNode();

    virtual void run(ExecutionNode *previous);
    virtual void setDiceKeepNumber(quint64 );
private:
    quint64 m_numberOfDice;
    DiceResult* m_diceResult;
};

#endif // KEEPDICEEXECNODE_H
