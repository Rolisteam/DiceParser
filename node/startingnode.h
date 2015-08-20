#ifndef STARTINGNODE_H
#define STARTINGNODE_H

#include "executionnode.h"

/**
 * @brief The StartingNode class is an ExecutionNode, StartingNode is dedicated to be the first node
 * in the execution tree.
 */
class StartingNode : public ExecutionNode
{
public:
    /**
     * @brief StartingNode
     */
    StartingNode();
    /**
     * @brief run
     */
    virtual void run(ExecutionNode*);
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool withlabel)const;
    /**
     * @brief getPriority
     * @return
     */
    virtual qint64 getPriority() const;
};

#endif // STARTINGNODE_H
