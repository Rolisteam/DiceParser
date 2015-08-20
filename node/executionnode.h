#ifndef EXECUTIONNODE_H
#define EXECUTIONNODE_H

#include "result/result.h"
#include <QDebug>
/**
 * @brief The ExecutionNode class
 */
class ExecutionNode
{
public:
    enum ERROR_CODE {NO_ERROR,DIE_RESULT_EXPECTED,BAD_SYNTAXE,ENDLESS_LOOP_ERROR,DIVIDE_BY_ZERO,NOTHING_UNDERSTOOD};
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
	virtual Result* getResult();
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
	 * @brief getPreviousNode
	 * @return
	 */
	virtual ExecutionNode* getPreviousNode() const;
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool withLabel)const=0;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const=0;
    /**
     * @brief getErrorList
     * @return
     */
    virtual QMap<ExecutionNode::ERROR_CODE,QString> getExecutionErrorMap();

    /**
     * @brief generateDotTree
     */
    virtual void generateDotTree(QString&);

    /**
     * @brief getHelp
     * @return
     */
    virtual QString getHelp();
protected:
	/**
	 * @brief m_nextNode
	 */
	ExecutionNode* m_previousNode;
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
    QMap<ExecutionNode::ERROR_CODE,QString> m_errors;
};

#endif // EXECUTIONNODE_H
