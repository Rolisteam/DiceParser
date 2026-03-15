#include "filternode.h"
#include "validatorlist.h"

FilterNode::FilterNode() : ExecutionNode("%1 [label=\"FilterNode\"]"), m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}

FilterNode::~FilterNode()
{
    if(nullptr != m_validatorList)
    {
        delete m_validatorList;
    }
}
void FilterNode::setValidatorList(ValidatorList* validatorlist)
{
    m_validatorList= validatorlist;
}
void FilterNode::run(ExecutionNode* previous)
{
    if(isValid(!previous, Dice::ERROR_CODE::NO_PREVIOUS_ERROR, tr("No Previous node")))
        return;
    m_previousNode= previous;

    DiceResult* previousDiceResult= dynamic_cast<DiceResult*>(previous->getResult());
    m_result->setPrevious(previousDiceResult);

    if(isValid(!previousDiceResult, Dice::ERROR_CODE::NO_VALID_RESULT, tr("No Valid result")))
        return;

    QList<Die*> diceList2;
    std::function<void(Die*, qint64)> f= [&diceList2](Die* die, qint64)
    {
        if(die == nullptr)
            return;
        Die* tmpdie= new Die(*die);
        diceList2.append(tmpdie);
        die->displayed();
    };
    m_validatorList->validResult(previousDiceResult, true, true, f);

    QList<Die*> diceList= previousDiceResult->getResultList();

    diceList.erase(
        std::remove_if(diceList.begin(), diceList.end(), [&diceList2](Die* die) { return diceList2.contains(die); }),
        diceList.end());
    for(Die* tmp : diceList)
    {
        tmp->setHighlighted(false);
        tmp->setDisplayed(true);
    }

    m_diceResult->setResultList(diceList2);
}

qint64 FilterNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_nextNode)
    {
        priority= m_nextNode->getPriority();
    }

    return priority;
}
ExecutionNode* FilterNode::getCopy() const
{
    FilterNode* node= new FilterNode();
    if(nullptr != m_validatorList)
    {
        node->setValidatorList(m_validatorList->getCopy());
    }
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
