#ifndef REROLLDICENODE_H
#define REROLLDICENODE_H


#include "executionnode.h"

/**
 * @brief The RerollDiceNode class reroll dice given a condition and replace(or add) the result.
 */
class RerollDiceNode : public ExecutionNode
{

public:
    enum ReRollMode {EQUAL,LESSER,GREATER};
    RerollDiceNode(ExecutionNode* previous);

    virtual void run();
private:
    ExecutionNode* m_previous;
};

#endif // REROLLDICENODE_H
