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
    enum DICE_ERROR_CODE {NO_DICE_ERROR,
                          DIE_RESULT_EXPECTED,
                          BAD_SYNTAXE,
                          ENDLESS_LOOP_ERROR,
                          DIVIDE_BY_ZERO,
                          NOTHING_UNDERSTOOD,
                          NO_DICE_TO_ROLL,
                          TOO_MANY_DICE,NO_VARIBALE,
                          INVALID_INDEX};
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
    virtual void run(ExecutionNode* previous = nullptr)=0;
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
    void setPreviousNode(ExecutionNode* node);
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
    virtual QMap<ExecutionNode::DICE_ERROR_CODE,QString> getExecutionErrorMap();

    /**
     * @brief generateDotTree
     */
    virtual void generateDotTree(QString&);

    /**
     * @brief getHelp
     * @return
     */
    virtual QString getHelp();

    /**
     * @brief getCopy
     * @return should return a copy of that node.
     */
    virtual ExecutionNode* getCopy() const  = 0;

    virtual qint64 getScalarResult();

protected:
    /**
     * @brief m_nextNode
     */
    ExecutionNode* m_previousNode= nullptr;
    /**
     * @brief m_result
     */
    Result* m_result;
    /**
     * @brief m_nextNode
     */
    ExecutionNode* m_nextNode = nullptr;
    /**
     * @brief m_errors
     */
    QMap<ExecutionNode::DICE_ERROR_CODE,QString> m_errors;

    QString m_id;
};

#endif // EXECUTIONNODE_H
