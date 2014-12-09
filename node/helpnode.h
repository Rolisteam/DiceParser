#ifndef HELPNODE_H
#define HELPNODE_H
#include "executionnode.h"

#include <QObject>
#include <QString>

#include "stringresult.h"

/**
 * @brief The HelpNode class
 */
class HelpNode : public ExecutionNode
{
public:
    /**
     * @brief HelpNode
     */
    HelpNode();
    /**
     * @brief run
     * @param previous
     */
    void run(ExecutionNode* previous);
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

};

#endif // HELPNODE_H
