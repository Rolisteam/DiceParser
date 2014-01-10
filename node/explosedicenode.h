#ifndef EXPLOSEDICENODE_H
#define EXPLOSEDICENODE_H

#include "executionnode.h"
#include "diceresult.h"
#include "validator.h"

/**
 * @brief The ExploseDiceNode class explose dice while is valid by the validator.
 */
class ExploseDiceNode : public ExecutionNode
{
public:
    ExploseDiceNode();
    virtual void run(ExecutionNode* previous = NULL);

    virtual void setValidator(Validator* );


protected:
    DiceResult* m_diceResult;
    Validator* m_validator;
};

#endif // EXPLOSEDICENODE_H
