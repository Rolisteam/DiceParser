#ifndef KEEPDICEEXECNODE_H
#define KEEPDICEEXECNODE_H

#include "executionnode.h"

class KeepDiceExecNode : public ExecutionNode
{
public:
    KeepDiceExecNode();

    virtual void run(ExecutionNode *previous);
    virtual void setDiceKeepNumber(quint64 );
private:
    quint64 m_numberOfDice;
};

#endif // KEEPDICEEXECNODE_H
