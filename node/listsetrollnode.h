#ifndef LISTSETROLLNODE_H
#define LISTSETROLLNODE_H


#include <QStringList>

#include "executionnode.h"
#include "result/diceresult.h"
#include "result/stringresult.h"

class ListSetRollNode : public ExecutionNode
{
public:
    ListSetRollNode();
    virtual void run(ExecutionNode* previous = NULL);
    virtual QString toString()const;
    virtual qint64 getPriority() const;
    QStringList getList();

    void setListValue(QStringList);

private:
    QStringList m_values;
    DiceResult* m_diceResult;
    StringResult* m_stringResult;

};

#endif // LISTSETROLLNODE_H
