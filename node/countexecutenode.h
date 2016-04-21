#ifndef COUNTEXECUTENODE_H
#define COUNTEXECUTENODE_H

#include "executionnode.h"

#include "validator.h"
#include "result/scalarresult.h"

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
	virtual ~CountExecuteNode();
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
    virtual QString toString(bool withLabel)const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;

private:
    ScalarResult* m_scalarResult;
    Validator* m_validator;
};

#endif // COUNTEXECUTENODE_H
