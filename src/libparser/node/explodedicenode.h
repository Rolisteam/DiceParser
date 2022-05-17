#ifndef EXPLOSEDICENODE_H
#define EXPLOSEDICENODE_H

#include "executionnode.h"
#include "result/diceresult.h"

class ValidatorList;

/**
 * @brief The ExplodeDiceNode class explode dice while is valid by the validator.
 */
class ExplodeDiceNode : public ExecutionNode
{
public:
    ExplodeDiceNode();
    virtual ~ExplodeDiceNode();
    virtual void run(ExecutionNode* previous= nullptr);
    virtual void setValidatorList(ValidatorList*);
    virtual QString toString(bool) const;
    virtual qint64 getPriority() const;

    virtual ExecutionNode* getCopy() const;

    void setLimitNode(ExecutionNode* limitNode);

protected:
    DiceResult* m_diceResult;
    ValidatorList* m_validatorList{nullptr};
    ExecutionNode* m_limit{nullptr};
};

#endif // EXPLOSEDICENODE_H
