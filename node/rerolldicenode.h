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
    RerollDiceNode();

    virtual void run(ExecutionNode* previous);
private:
    qint64 m_value;
};

#endif // REROLLDICENODE_H
