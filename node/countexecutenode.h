#ifndef COUNTEXECUTENODE_H
#define COUNTEXECUTENODE_H

#include "executionnode.h"

#include "validator.h"
#include "scalarresult.h"

/**
 * @brief The CountExecuteNode class
 */
class CountExecuteNode : public ExecutionNode
{
public:
    /**
     * @brief CountExecuteNode
     */
    CountExecuteNode();
    /**
     * @brief run
     * @param previous
     */
    virtual void run(ExecutionNode* previous);
    /**
     * @brief setValidator
     */
    virtual void setValidator(Validator* );
    /**
     * @brief toString
     * @return
     */
    virtual QString toString()const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;

private:
    Validator* m_validator;
    ScalarResult* m_scalarResult;
};

#endif // COUNTEXECUTENODE_H
