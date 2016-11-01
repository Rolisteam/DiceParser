#ifndef STRINGNODE_H
#define STRINGNODE_H

#include "node/executionnode.h"
#include "result/stringresult.h"

/**
 * @brief The StringNode class is an ExecutionNode. It is dedicated to store string and display result.
 */
class StringNode : public ExecutionNode
{
public:
    StringNode();
    void run(ExecutionNode* previous);
    void setString(QString str);
    virtual QString toString(bool withLabel)const;
    virtual qint64 getPriority() const;

private:
    QString m_data;
    StringResult* m_stringResult;

};

#endif // STRINGNODE_H
