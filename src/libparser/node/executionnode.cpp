#include "executionnode.h"

#include <QUuid>

namespace
{
#ifdef QT_DEBUG
constexpr int WaitingTime{2 * 1000 * 60};
#else
constexpr int WaitingTime{30 * 60 * 1000};
#endif

} // namespace

ExecutionNode::ExecutionNode(const QString& label)
    : m_previousNode(nullptr)
    , m_result(nullptr)
    , m_nextNode(nullptr)
    , m_errors(QMap<Dice::ERROR_CODE, QString>())
    , m_id(QString("\"%1\"").arg(QUuid::createUuid().toString()))
    , m_label(label)
{
}
ExecutionNode::~ExecutionNode()
{
    if(nullptr != m_result)
    {
        delete m_result;
        m_result= nullptr;
    }
    if(nullptr != m_nextNode)
    {
        delete m_nextNode;
        m_nextNode= nullptr;
    }
}

Result* ExecutionNode::getResult()
{
    return m_result;
}
void ExecutionNode::setNextNode(ExecutionNode* node)
{
    m_nextNode= node;
}
void ExecutionNode::setPreviousNode(ExecutionNode* node)
{
    m_previousNode= node;
}

QString ExecutionNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return m_label.arg(m_id);
    }
    else
    {
        return m_id;
    }
}

ExecutionNode* ExecutionNode::getNextNode()
{
    return m_nextNode;
}

void ExecutionNode::execute(ExecutionNode* previous)
{
    QElapsedTimer timer;
    timer.start();

    auto errorCount= m_errors.count();

    run(previous);

    auto timeLimit= timer.hasExpired(WaitingTime);

    if(m_nextNode && errorCount == m_errors.count() && !timeLimit)
        m_nextNode->execute(this);
    else if(timeLimit)
        qDebug() << "Error too long" << WaitingTime << timeLimit << timer.elapsed();
}

QMap<Dice::ERROR_CODE, QString> ExecutionNode::getExecutionErrorMap()
{
    if(nullptr != m_nextNode)
    {
        auto const& keys= m_nextNode->getExecutionErrorMap().keys();
        for(auto& key : keys)
        {
            m_errors.insert(key, m_nextNode->getExecutionErrorMap().value(key));
        }
    }
    return m_errors;
}
QString ExecutionNode::getHelp()
{
    return QString();
}
ExecutionNode* ExecutionNode::getPreviousNode() const
{
    return m_previousNode;
}
void ExecutionNode::generateDotTree(QString& s)
{
    auto str= toString(true);
    if(s.contains(str))
        return;
    s.append(toString(true));
    s.append(";\n");

    if(nullptr != m_nextNode)
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append(m_nextNode->toString(false));
        s.append("[label=\"next\"];\n");
        //        s.append(" [label=\"nextNode\"];\n");
        m_nextNode->generateDotTree(s);
    }
    else
    {
        s.append(toString(false));
        s.append(" -> ");
        s.append("nullptr;\n");
    }
    if(nullptr != m_result)
    {
        s.append(toString(false));
        s.append(" ->");
        s.append(m_result->toString(false));
        s.append(" [label=\"Result\", style=\"dashed\"];\n");
        if(nullptr == m_nextNode)
            m_result->generateDotTree(s);
    }
}
qint64 ExecutionNode::getScalarResult()
{
    if(m_result == nullptr)
        return 0;
    return m_result->getResult(Dice::RESULT_TYPE::SCALAR).toInt();
}

bool ExecutionNode::isValid(bool condition, Dice::ERROR_CODE code, const QString& errorTxt, bool error)
{
    if(condition)
        error ? m_errors.insert(code, errorTxt) : m_warnings.insert(code, errorTxt);

    return condition;
}
