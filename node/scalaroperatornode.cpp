#include "scalaroperatornode.h"

#include <QDebug>
#include "diceresult.h"


ScalarOperatorNode::ScalarOperatorNode()
    : m_internalNode(NULL),m_myScalarResult(new ScalarResult())
{
    m_scalarOperationList.insert('+',PLUS);
    m_scalarOperationList.insert('-',MINUS);
    m_scalarOperationList.insert('x',MULTIPLICATION);
    m_scalarOperationList.insert('*',MULTIPLICATION);
    m_scalarOperationList.insert('/',DIVIDE);

    m_result = m_myScalarResult;
}

void ScalarOperatorNode::run(ExecutionNode* previous)
{

    if(NULL!=m_internalNode)
    {
        m_internalNode->run(this);
    }
    if(NULL!=previous)
    {
        DiceResult* previousResult = static_cast<DiceResult*>(previous->getResult());

        if(NULL!=previousResult)
        {
            ExecutionNode* internal = m_internalNode;
            while(NULL != internal->getNextNode() )
            {
                internal = internal->getNextNode();
            }

            Result* internalResult = internal->getResult();
            m_result->setPrevious(internalResult);
            internalResult->setPrevious(previousResult);

            qDebug() << "ScalarOperatorNode node" << m_myOperator;

            switch(m_myOperator)
            {
                case PLUS:
                    m_myScalarResult->setValue(add(previousResult->getScalar(),internalResult->getScalar()));
                    break;
                case MINUS:
                    m_myScalarResult->setValue(substract(previousResult->getScalar(),internalResult->getScalar()));
                    break;
                case MULTIPLICATION:
                    m_myScalarResult->setValue(multiple(previousResult->getScalar(),internalResult->getScalar()));
                    break;
                case DIVIDE:
                    m_myScalarResult->setValue(divide(previousResult->getScalar(),internalResult->getScalar()));
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
QString ScalarOperatorNode::toString() const
{
    return "ScalarOperatorNode";
}
