#ifndef VARIABLENODE_H
#define VARIABLENODE_H

#include "node/executionnode.h"

/**
 * @brief The VariableNode class is an ExecutionNode. It is dedicated to retrive
 *  variable value from other starting node.
 */
class VariableNode : public ExecutionNode
{
public:
    VariableNode();
    void run(ExecutionNode* previous);
    virtual QString toString(bool withLabel)const;
    virtual qint64 getPriority() const;
    /**
     * @brief getCopy
     * @return
     */
    virtual ExecutionNode *getCopy() const;
    qint64 getIndex() const;
    void setIndex(qint64 index);

    std::vector<ExecutionNode *> getData() const;
    void setData(const std::vector<ExecutionNode *> &data);

private:
    qint64 m_index;
    std::vector<ExecutionNode*> m_data;
};

#endif // VARIABLENODE_H
