#include "filternode.h"

FilterNode::FilterNode()
    : m_diceResult(new DiceResult()),m_eachValue(false)
{
    m_result = m_diceResult;
}

FilterNode::~FilterNode()
{
    if(NULL!=m_validator)
    {
        delete m_validator;
    }
}
void FilterNode::setValidator(Validator* validator)
{
    m_validator = validator;
}
void FilterNode::run(ExecutionNode* previous)
{
    m_previousNode = previous;
    if(NULL==previous)
    {
        return;
    }
    DiceResult* previousDiceResult = static_cast<DiceResult*>(previous->getResult());
    m_result->setPrevious(previousDiceResult);
    if(NULL!=previousDiceResult)
    {
        QList<Die*> diceList=previousDiceResult->getResultList();
        QList<Die*> diceList2;


        for(Die* tmp : diceList)
        {
            if(m_validator->hasValid(tmp,m_eachValue))
            {
                Die* tmpdie = new Die();
                *tmpdie=*tmp;
                diceList2.append(tmpdie);
                tmp->displayed();
            }
            else
            {
                tmp->setHighlighted(false);
            }
        }

        m_diceResult->setResultList(diceList2);
        if(NULL!=m_nextNode)
        {
            m_nextNode->run(this);
        }
    }
}

QString FilterNode::toString(bool wl) const
{
    if(wl)
    {
        return QString("%1 [label=\"FilterNode\"]").arg(m_id);
    }
    else
    {
        return m_id;
    }
}
qint64 FilterNode::getPriority() const
{
    qint64 priority=0;
    if(nullptr!=m_previousNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
ExecutionNode* FilterNode::getCopy() const
{
    FilterNode* node = new FilterNode();
    if(NULL!=m_validator)
    {
        node->setValidator(m_validator->getCopy());
    }
    if(NULL!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
