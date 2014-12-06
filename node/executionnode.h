#ifndef EXECUTIONNODE_H
#define EXECUTIONNODE_H

#include "result.h"
#include <QDebug>
/**
 * @brief The ExecutionNode class
 */
class ExecutionNode
{
public:
    enum ERROR_CODE {NO_ERROR,DIE_RESULT_EXPECTED,BAD_SYNTAXE};
    /**
     * @brief ExecutionNode
     */
    ExecutionNode();
    /**
     * @brief ~ExecutionNode
     */
    virtual ~ExecutionNode();
    /**
     * @brief run
     * @param previous
     */
    virtual void run(ExecutionNode* previous = NULL)=0;
    /**
     * @brief getResult
     * @return
     */
    Result* getResult();
    /**
     * @brief setNextNode
     */
    void setNextNode(ExecutionNode*);
    /**
     * @brief getNextNode
     * @return
     */
    ExecutionNode* getNextNode();
    /**
     * @brief toString
     * @return
     */
    virtual QString toString()const=0;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const=0;
    /**
     * @brief getErrorList
     * @return
     */
    virtual QList<ExecutionNode::ERROR_CODE> getErrorList();
protected:
    /**
     * @brief m_result
     */
    Result* m_result;
    /**
     * @brief m_nextNode
     */
    ExecutionNode* m_nextNode;
    /**
     * @brief m_errors
     */
    QList<ExecutionNode::ERROR_CODE> m_errors;
};

#endif // EXECUTIONNODE_H
