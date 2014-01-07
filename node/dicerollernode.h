#ifndef DICEROLLERNODE_H
#define DICEROLLERNODE_H

#include <Qt>

#include "executionnode.h"
#include "diceresult.h"

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
    DiceResult* m_myDiceResult;

};

#endif // DICEROLLERNODE_H
