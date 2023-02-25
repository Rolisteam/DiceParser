#ifndef EXECUTIONNODE_H
#define EXECUTIONNODE_H

#include "result/result.h"
#include <diceparser/diceparser_global.h>
#include <diceparser/diceparserhelper.h>

#include <QCoreApplication>
/**
 * @brief The ExecutionNode class
 */
class DICEPARSER_EXPORT ExecutionNode
{
    Q_DECLARE_TR_FUNCTIONS(ExecutionNode)
public:
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
    virtual void run(ExecutionNode* previous= nullptr)= 0;
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
    virtual QString toString(bool withLabel) const= 0;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const= 0;
    /**
     * @brief getErrorList
     * @return
     */
    virtual QMap<Dice::ERROR_CODE, QString> getExecutionErrorMap();

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
    virtual ExecutionNode* getCopy() const= 0;

    virtual qint64 getScalarResult();

protected:
    /**
     * @brief m_nextNode
     */
    ExecutionNode* m_previousNode= nullptr;
    /**
     * @brief m_result
     */
    Result* m_result= nullptr;
    /**
     * @brief m_nextNode
     */
    ExecutionNode* m_nextNode= nullptr;
    /**
     * @brief m_errors
     */
    QMap<Dice::ERROR_CODE, QString> m_errors;

    QString m_id;
};

#endif // EXECUTIONNODE_H
