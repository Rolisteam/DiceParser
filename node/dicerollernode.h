#ifndef DICEROLLERNODE_H
#define DICEROLLERNODE_H

#include "executionnode.h"

class DiceRollerNode : public ExecutionNode
{
public:
    DiceRollerNode(quint64 faces);

    virtual void run(ExecutionNode*);

    //private method
private:
    quint64 rollDice();


//private members
private:
    quint64 m_diceCount;
    quint64 m_faces; /// faces

};

#endif // DICEROLLERNODE_H
