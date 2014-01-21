#ifndef REROLLDICENODE_H
#define REROLLDICENODE_H


#include "executionnode.h"
#include "diceresult.h"
#include "validator.h"
/**
 * @brief The RerollDiceNode class reroll dice given a condition and replace(or add) the result.
 */
class RerollDiceNode : public ExecutionNode
{

public:
    enum ReRollMode {EQUAL,LESSER,GREATER};
    RerollDiceNode();

    virtual void run(ExecutionNode* previous);

    virtual void setValidator(Validator* );
    virtual QString toString()const;

private:
    Validator* m_validator;
    DiceResult* m_myDiceResult;
};

#endif // REROLLDICENODE_H
