#include "variablenode.h"
#include "parsingtoolbox.h"
VariableNode::VariableNode() {}

void VariableNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if((nullptr != m_data) && (m_data->size() > m_index))
    {
        auto value= (*m_data)[m_index];
        value= ParsingToolBox::getLeafNode(value);
        if(nullptr == value)
            return;

        auto result= value->getResult();
        if(!result)
            return;

        auto copy= result->getCopy();
        auto diceResult= dynamic_cast<DiceResult*>(result);
        if(nullptr == diceResult)
            return;

        for(auto& die : diceResult->getResultList())
        {
            die->setDisplayed(false);
        }

        m_result= copy;
        if(nullptr != m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
    else
    {
        m_errors.insert(Dice::ERROR_CODE::NO_VARIBALE, QObject::tr("No variable at index:%1").arg(m_index + 1));
    }
}

void VariableNode::setDisplayed()
{
    if(!m_result)
        return;
    auto diceResult= dynamic_cast<DiceResult*>(m_result);
    if(nullptr == diceResult)
        return;

    for(auto& die : diceResult->getResultList())
    {
        die->setDisplayed(true);
    }
}

QString VariableNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return QString("%1 [label=\"VariableNode index: %2\"]").arg(m_id).arg(m_index + 1);
    }
    else
    {
        return m_id;
    }
}

qint64 VariableNode::getPriority() const
{
    qint64 priority= 4;
    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}

ExecutionNode* VariableNode::getCopy() const
{
    VariableNode* node= new VariableNode();
    node->setIndex(m_index);
    if(nullptr != m_data)
    {
        node->setData(m_data);
    }
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

quint64 VariableNode::getIndex() const
{
    return m_index;
}

void VariableNode::setIndex(quint64 index)
{
    m_index= index;
}

std::vector<ExecutionNode*>* VariableNode::getData() const
{
    return m_data;
}

void VariableNode::setData(std::vector<ExecutionNode*>* data)
{
    m_data= data;
}
