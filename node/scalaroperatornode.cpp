#include "scalaroperatornode.h"

#include <QDebug>
#include "result/diceresult.h"


ScalarOperatorNode::ScalarOperatorNode()
    : m_internalNode(NULL),m_scalarResult(new ScalarResult())
{
    m_scalarOperationList.insert('+',PLUS);
    m_scalarOperationList.insert('-',MINUS);
    m_scalarOperationList.insert('x',MULTIPLICATION);
    m_scalarOperationList.insert('*',MULTIPLICATION);
    m_scalarOperationList.insert('/',DIVIDE);

    m_result = m_scalarResult;
}
ScalarOperatorNode::~ScalarOperatorNode()
{
	if(NULL!=m_internalNode)
	{
		delete m_internalNode;
		m_internalNode = NULL;
	}
}

void ScalarOperatorNode::run(ExecutionNode* previous)
{
	m_previousNode = previous;
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
            if(NULL!=m_internalNode->getResult())
            {
                m_internalNode->getResult()->setPrevious(previousResult);
            }

			switch(m_operator)
            {
                case PLUS:
                    m_scalarResult->setValue(add(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal()));
                    break;
                case MINUS:
                    m_scalarResult->setValue(substract(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal()));
                    break;
                case MULTIPLICATION:
                    m_scalarResult->setValue(multiple(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal()));
                    break;
                case DIVIDE:
                    m_scalarResult->setValue(divide(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal()));
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
		m_operator = m_scalarOperationList.value(c);
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

qreal ScalarOperatorNode::divide(qint64 a,qint64 b)
{
    return (qreal)a/b;
}

qint64 ScalarOperatorNode::multiple(qint64 a,qint64 b)
{
    return a*b;
}
QString ScalarOperatorNode::toString() const
{
    return "ScalarOperatorNode";
}
qint64 ScalarOperatorNode::getPriority() const
{
	if((m_operator==PLUS)||(m_operator==MINUS))
	{
        return 1;
	}
    else
	{
        return 2;
	}
}
void ScalarOperatorNode::generateDotTree(QString& s)
{
    s.append(toString());
    if(NULL!=m_nextNode)
    {
        s.append(" -> ");
        s.append(m_nextNode->toString());
        s.append(" [label=\"nextNode\"];\n");
        m_nextNode->generateDotTree(s);
    }
    else
    {
        s.append(" -> ");
        s.append("NULL");
        s.append(" [label=\"nextNode\"];\n");
    }
    QString str;
    str.append("\n");
    if(NULL!=m_internalNode)
    {
        str.append(toString());
        str.append(" -> ");
        str.append(m_internalNode->toString());
        str.append(" [label=\"internalNode\"];\n");
        m_internalNode->generateDotTree(str);
    }
    s.append(str);
}
