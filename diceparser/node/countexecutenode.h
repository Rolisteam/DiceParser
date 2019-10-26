#ifndef COUNTEXECUTENODE_H
#define COUNTEXECUTENODE_H

#include "executionnode.h"

#include "result/scalarresult.h"

class ValidatorList;
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
    /**
     * @brief getCopy
     * @return
     */
    virtual ExecutionNode* getCopy() const;

private:
    ScalarResult* m_scalarResult;
    ValidatorList* m_validatorList;
};

#endif // COUNTEXECUTENODE_H
