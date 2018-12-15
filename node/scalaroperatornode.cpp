/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://www.rolisteam.org/contact                      *
*                                                                          *
*  This file is part of DiceParser                                         *
*                                                                          *
* DiceParser is free software; you can redistribute it and/or modify       *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation; either version 2 of the License, or        *
* (at your option) any later version.                                      *
*                                                                          *
* This program is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with this program; if not, write to the                            *
* Free Software Foundation, Inc.,                                          *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 *
***************************************************************************/
#include "scalaroperatornode.h"

#include <QDebug>
#include "result/diceresult.h"


ScalarOperatorNode::ScalarOperatorNode()
    : m_internalNode(nullptr),m_scalarResult(new ScalarResult()),m_arithmeticOperator(Die::PLUS)
{
    m_result = m_scalarResult;
}
ScalarOperatorNode::~ScalarOperatorNode()
{
    if(nullptr!=m_internalNode)
    {
        delete m_internalNode;
        m_internalNode = nullptr;
    }
}

void ScalarOperatorNode::run(ExecutionNode* previous)
{
    m_previousNode = previous;
    if(nullptr!=m_internalNode)
    {
        m_internalNode->run(this);
    }
    if(nullptr!=previous)
    {
        auto previousResult = previous->getResult();

        if(nullptr!=previousResult)
        {
            ExecutionNode* internal = m_internalNode;
            if(nullptr != internal)
            {
                while(nullptr != internal->getNextNode() )
                {
                    internal = internal->getNextNode();
                }

                Result* internalResult = internal->getResult();
                m_result->setPrevious(internalResult);
                if(nullptr!=m_internalNode->getResult())
                {
                    m_internalNode->getResult()->setPrevious(previousResult);
                }

                switch(m_arithmeticOperator)
                {
                case Die::PLUS:
                    m_scalarResult->setValue(add(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal()));
                    break;
                case Die::MINUS:
                    m_scalarResult->setValue(substract(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal()));
                    break;
                case Die::MULTIPLICATION:
                    m_scalarResult->setValue(multiple(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal()));
                    break;
                case Die::DIVIDE:
                    m_scalarResult->setValue(divide(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal()));
                    break;
                case Die::INTEGER_DIVIDE:
                    m_scalarResult->setValue(static_cast<int>(divide(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal())));
                    break;
                case Die::POW:
                    m_scalarResult->setValue(pow(previousResult->getResult(Result::SCALAR).toReal(),internalResult->getResult(Result::SCALAR).toReal()));
                    break;

                }
            }

            if(nullptr!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
    }

}
/*bool ScalarOperatorNode::setOperatorChar(QChar c)
{
    if(m_scalarOperationList.contains(c))
    {
        m_operator = m_scalarOperationList.value(c);
        return true;
    }
    return false;
}*/

void ScalarOperatorNode::setInternalNode(ExecutionNode* node)
{
    m_internalNode = node;
}
qint64 ScalarOperatorNode::add(qreal a,qreal b)
{
    return static_cast<qint64>(a+b);
}
qint64 ScalarOperatorNode::substract(qreal a,qreal b)
{
    return static_cast<qint64>(a-b);
}
qreal ScalarOperatorNode::divide(qreal a,qreal b)
{
    if(qFuzzyCompare(b,0))
    {
        m_errors.insert(DIVIDE_BY_ZERO,QObject::tr("Division by zero"));
        return 0;
    }
    return static_cast<qreal>(a/b);
}
qint64 ScalarOperatorNode::multiple(qreal a,qreal b)
{
    return static_cast<qint64>(a*b);
}
qint64 ScalarOperatorNode::pow(qreal a,qreal b)
{
    return static_cast<qint64>(std::pow(a,b));
}
Die::ArithmeticOperator ScalarOperatorNode::getArithmeticOperator() const
{
    return m_arithmeticOperator;
}

void ScalarOperatorNode::setArithmeticOperator(const Die::ArithmeticOperator &arithmeticOperator)
{
    m_arithmeticOperator = arithmeticOperator;
}

QString ScalarOperatorNode::toString(bool wl) const
{
    QString op="";
    switch(m_arithmeticOperator)
    {
    case Die::PLUS:
        op="+";
        break;
    case Die::MINUS:
        op="-";
        break;
    case Die::MULTIPLICATION:
        op="*";
        break;
    case Die::DIVIDE:
        op="/";
        break;
    case Die::INTEGER_DIVIDE:
        op="|";
        break;
    case Die::POW:
        op="^";
        break;
    }
    if(wl)
    {
        return QString("%1 [label=\"ScalarOperatorNode %2\"]").arg(m_id).arg(op);
    }
    else
    {
        return m_id;
    }
}
qint64 ScalarOperatorNode::getPriority() const
{
    if((m_arithmeticOperator==Die::PLUS)||(m_arithmeticOperator==Die::MINUS))
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
    auto id = toString(true);
    if(s.contains(id))
        return;
    s.append(id);
    s.append(";\n");

    if(nullptr!=m_nextNode)
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_nextNode->toString(false));
        s.append("[label=\"nextNode\"];\n");
        m_nextNode->generateDotTree(s);
    }
    else
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append("nullptr");
        s.append(" [label=\"nextNode\"];\n");
    }

    if(nullptr!=m_result)
    {
        s.append(toString(false));
        s.append(" ->");
        s.append(m_result->toString(false));
        s.append(" [label=\"Result\", style=\"dashed\"];\n");
        if(nullptr == m_nextNode)
            m_result->generateDotTree(s);
    }
    QString str;
    str.append("\n");
    if(nullptr!=m_internalNode)
    {
        str.append(toString(false));
        str.append(" -> ");
        str.append(m_internalNode->toString(false));
        str.append(" [label=\"internalNode\"];\n");
        m_internalNode->generateDotTree(str);
    }
    s.append(str);
}
QMap<ExecutionNode::DICE_ERROR_CODE,QString> ScalarOperatorNode::getExecutionErrorMap()
{
    if(nullptr!=m_internalNode)
    {
        for (ExecutionNode::DICE_ERROR_CODE key: m_internalNode->getExecutionErrorMap().keys())
        {
            m_errors.insert(key,m_internalNode->getExecutionErrorMap().value(key));
        }
    }
    if(nullptr!=m_nextNode)
    {
        for (ExecutionNode::DICE_ERROR_CODE key: m_nextNode->getExecutionErrorMap().keys())
        {
            m_errors.insert(key,m_nextNode->getExecutionErrorMap().value(key));
        }
    }
    return m_errors;
}
ExecutionNode* ScalarOperatorNode::getCopy() const
{
    ScalarOperatorNode* node = new ScalarOperatorNode();
    node->setInternalNode(m_internalNode->getCopy());
    node->setArithmeticOperator(m_arithmeticOperator);
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
