#include "scalaroperatornode.h"

#include <QDebug>

ScalarOperatorNode::ScalarOperatorNode()
    : m_internalNode(NULL)
{
    m_scalarOperationList.insert('+',PLUS);
    m_scalarOperationList.insert('-',MINUS);
    m_scalarOperationList.insert('x',MULTIPLICATION);
    m_scalarOperationList.insert('*',MULTIPLICATION);
    m_scalarOperationList.insert('/',DIVIDE);
    m_scalarOperationList.insert('÷',DIVIDE);
}

void ScalarOperatorNode::run(ExecutionNode* previous)
{
    if(NULL!=m_internalNode)
    {
        m_internalNode->run(this);
    }
    if(NULL!=previous)
    {
        DiceResult* previousResult = previous->getResult();
        ExecutionNode* internal = m_internalNode;
        while(NULL != internal->getNextNode() )
        {
            internal = internal->getNextNode();
        }
         DiceResult* internalResult = internal->getResult();

         switch(m_myOperator)
         {
         case PLUS:
             m_result.insertResult(add(previousResult->getSum(),internalResult->getSum()));
             break;
         case MINUS:
             m_result.insertResult(substract(previousResult->getSum(),internalResult->getSum()));
             break;
         case MULTIPLICATION:
             m_result.insertResult(multiple(previousResult->getSum(),internalResult->getSum()));
             break;
         case DIVIDE:
             m_result.insertResult(divide(previousResult->getSum(),internalResult->getSum()));
             break;
         default:
             break;

         }

        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }

}
bool ScalarOperatorNode::setOperatorChar(QChar c)
{
    if(m_scalarOperationList.contains(c))
    {
        m_myOperator = m_scalarOperationList.value(c);
        return true;
    }
    return false;
}


void ScalarOperatorNode::setInternalNode(ExecutionNode* node)
{
    m_internalNode = node;
}
qint64 ScalarOperatorNode::add(qint64 a,qint64 b)
{
    return a+b;
}

qint64 ScalarOperatorNode::substract(qint64 a,qint64 b)
{
    return a-b;
}

qint64 ScalarOperatorNode::divide(qint64 a,qint64 b)
{
    return a/b;
}

qint64 ScalarOperatorNode::multiple(qint64 a,qint64 b)
{
    return a*b;
}
