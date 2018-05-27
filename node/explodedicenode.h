#ifndef EXPLOSEDICENODE_H
#define EXPLOSEDICENODE_H

#include "executionnode.h"
#include "result/diceresult.h"
#include "validator.h"
#include <QDebug>

/**
 * @brief The ExplodeDiceNode class explode dice while is valid by the validator.
 */
class ExplodeDiceNode : public ExecutionNode
{
public:
    ExplodeDiceNode();
	virtual ~ExplodeDiceNode();
    virtual void run(ExecutionNode* previous = nullptr);
    virtual void setValidator(Validator* );
	virtual QString toString(bool )const;
    virtual qint64 getPriority() const;

    virtual ExecutionNode *getCopy() const;
protected:
    DiceResult* m_diceResult;
    Validator* m_validator;
};

#endif // EXPLOSEDICENODE_H
