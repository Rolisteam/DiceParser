#ifndef OCCURENCECOUNTNODE_H
#define OCCURENCECOUNTNODE_H

#include "executionnode.h"
#include "validator.h"

class StringResult;
class OccurenceCountNode : public ExecutionNode
{
public:
    OccurenceCountNode();

    void run(ExecutionNode* previous = nullptr);
    virtual QString toString(bool withLabel)const;

    ExecutionNode* getCopy() const;
    qint64 getPriority() const;

    qint64 getWidth() const;
    void setWidth(const qint64 &width);

    qint64 getHeight() const;
    void setHeight(const qint64 &height);

    Validator *getValidator() const;
    void setValidator(Validator *validator);

private:
    qint64 m_width=0;
    qint64 m_height=0;
    Validator* m_validator;
    StringResult* m_stringResult;
};

#endif // OCCURENCECOUNTNODE_H
