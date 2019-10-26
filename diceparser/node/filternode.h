#ifndef FILTERNODE_H
#define FILTERNODE_H

#include "executionnode.h"

#include "result/diceresult.h"

class ValidatorList;

class FilterNode : public ExecutionNode
{
public:
    FilterNode();
    virtual ~FilterNode();

    virtual void run(ExecutionNode* previous);
    /**
     * @brief setValidator
     */
    virtual void setValidatorList(ValidatorList*);
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool withLabel) const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;

    virtual ExecutionNode* getCopy() const;

private:
    DiceResult* m_diceResult;
    ValidatorList* m_validatorList;
    bool m_eachValue;
};

#endif // FILTERNODE_H
