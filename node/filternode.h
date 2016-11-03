#ifndef FILTERNODE_H
#define FILTERNODE_H

#include "executionnode.h"

#include "validator.h"
#include "result/diceresult.h"

class FilterNode : public ExecutionNode
{
public:
    FilterNode();
    virtual ~FilterNode();

    virtual void run(ExecutionNode* previous);
    /**
     * @brief setValidator
     */
    virtual void setValidator(Validator* );
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool withLabel)const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;

private:
    DiceResult* m_diceResult;
    Validator* m_validator;
    bool m_eachValue;
};

#endif // FILTERNODE_H
