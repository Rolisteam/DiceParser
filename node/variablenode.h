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
    void run(ExecutionNode* previous) override;
    virtual QString toString(bool withLabel) const override;
    virtual qint64 getPriority() const override;
    /**
     * @brief getCopy
     * @return
     */
    virtual ExecutionNode* getCopy() const override;
    quint64 getIndex() const;
    void setIndex(quint64 index);

    std::vector<ExecutionNode*>* getData() const;
    void setData(std::vector<ExecutionNode*>* data);

    void setDisplayed();

private:
    quint64 m_index;
    std::vector<ExecutionNode*>* m_data= nullptr;
};

#endif // VARIABLENODE_H
