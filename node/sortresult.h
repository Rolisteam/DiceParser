#ifndef SORTRESULT_H
#define SORTRESULT_H

#include "executionnode.h"
#include "diceresult.h"

class SortResultNode : public ExecutionNode
{
public:
    SortResultNode();
    virtual void run(ExecutionNode*);


    void setSortAscending(bool asc);
private:
    bool m_ascending;
    DiceResult* m_diceResult;
};

#endif // SORTRESULT_H