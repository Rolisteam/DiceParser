#ifndef EXECUTIONNODE_H
#define EXECUTIONNODE_H

#include "result/result.h"
#include <diceparser/diceparser_global.h>
#include <diceparser/diceparserhelper.h>

#include <QCoreApplication>
#include <QElapsedTimer>
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
    ExecutionNode(const QString& label= QString());
    /**
     * @brief ~ExecutionNode
     */
    virtual ~ExecutionNode();

    virtual void execute(ExecutionNode* previous= nullptr);
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
    virtual void setNextNode(ExecutionNode*);
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
    virtual QString toString(bool withLabel) const;
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

    bool isValid(bool condition, Dice::ERROR_CODE code, const QString& errorTxt, bool error= true);

protected:
    /**
     * @brief previous
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
    QMap<Dice::ERROR_CODE, QString> m_warnings;
    QString m_id;
    QString m_label;
};

#endif // EXECUTIONNODE_H
