#include <diceparser/parsingtoolbox.h>

#include "booleancondition.h"
#include "rerolldicenode.h"
#include "validatorlist.h"
#include <utility>

RerollDiceNode::RerollDiceNode(bool reroll, bool addingMode)
    : m_diceResult(new DiceResult()), m_validatorList(nullptr), m_reroll(reroll), m_adding(addingMode)
{
    m_result= m_diceResult;
}
RerollDiceNode::~RerollDiceNode()
{
    if(nullptr != m_validatorList)
    {
        delete m_validatorList;
        m_validatorList= nullptr;
    }
}
void RerollDiceNode::run(ExecutionNode* previous)
{
    using DE= Dice::ERROR_CODE;
    using DC= Dice::CONDITION_STATE;
    if(isValid(!previous, DE::NO_PREVIOUS_ERROR, tr("No Previous node")))
        return;

    m_previousNode= previous;

    auto previousDiceResult= previous->getResult();
    if(isValid(!previousDiceResult, DE::NO_VALID_RESULT, tr("No Valid result")))
        return;

    DiceResult* previous_result= dynamic_cast<DiceResult*>(previousDiceResult);
    if(isValid(!previous_result, DE::DIE_RESULT_EXPECTED,
               tr(" The a operator expects dice result. Please check the documentation and fix your command.")))
        return;

    m_result->setPrevious(previous_result);

    QList<Die*>& list1= previous_result->getResultList();

    auto allInvalid= std::all_of(std::begin(list1), std::end(list1),
                                 [](const Die* die) { return die->getMaxValue() < die->getBase(); });

    qint64 maxVal= 0;
    if(allInvalid)
    {

        auto const& list1= m_validatorList->validators();
        auto max= std::max_element(std::begin(list1), std::end(list1),
                                   [](Validator* a, Validator* b)
                                   {
                                       auto aa= dynamic_cast<BooleanCondition*>(a);
                                       auto bb= dynamic_cast<BooleanCondition*>(b);

                                       if(bb && aa)
                                           return aa->valueToScalar() < bb->valueToScalar();
                                       else
                                           return (!bb && aa);
                                   });

        auto maxCondition= dynamic_cast<BooleanCondition*>(*max);
        if(maxCondition)
            maxVal= maxCondition->valueToScalar();
    }

    for(auto const& die : std::as_const(list1))
    {
        if(!die)
            return;

        Die* tmpdie= new Die(*die);
        m_diceResult->insertResult(tmpdie);
        die->displayed();

        if(allInvalid && maxVal != tmpdie->getMaxValue() && maxVal > tmpdie->getBase())
        {
            qInfo() << "Invalid range for explosing dice, set " << maxVal << " as maximum" << tmpdie->getMaxValue()
                    << tmpdie->getBase();
            tmpdie->setMaxValue(maxVal);
        }
    }

    QList<Die*>& list= m_diceResult->getResultList();
    QList<Die*> toRemove;

    for(int i= 0; i < list.size(); ++i)
    {
        auto die= list[i];
        if(!die)
            continue;

        bool finished= false;
        auto state
            = m_validatorList->isValidRangeSize(std::make_pair<qint64, qint64>(die->getBase(), die->getMaxValue()));

        if(isValid((DC::ALWAYSTRUE == state && m_adding) || (!m_reroll && !m_adding && state == DC::UNREACHABLE),
                   DE::ENDLESS_LOOP_ERROR,
                   QStringLiteral("Condition cause an endless loop with this dice: %1")
                       .arg(QStringLiteral("d[%1,%2]")
                                .arg(static_cast<int>(die->getBase()))
                                .arg(static_cast<int>(die->getMaxValue())))))
        {
            continue;
        }

        while(m_validatorList->hasValid(die, false) && !finished)
        {
            if(m_instruction)
            {
                m_instruction->execute(this);
                auto lastNode= ParsingToolBox::getLeafNode(m_instruction);
                if(lastNode == nullptr)
                    continue;
                auto lastResult= dynamic_cast<DiceResult*>(lastNode->getResult());
                if(lastResult != nullptr)
                {
                    toRemove.append(die);
                    if(list.size() < lastResult->getResultList().size())
                        break;
                    list.append(lastResult->getResultList());
                    die= list[i];
                    lastResult->clear();
                }
            }
            else
            {
                die->roll(m_adding);
            }
            finished= m_reroll;
        }
    }

    for(auto die : toRemove)
    {
        list.removeOne(die);
    }
}
void RerollDiceNode::setValidatorList(ValidatorList* val)
{
    m_validatorList= val;
}
QString RerollDiceNode::toString(bool wl) const
{
    return wl ? QString("%1 [label=\"RerollDiceNode validatior: %2\"]").arg(m_id, m_validatorList->toString()) :
                ExecutionNode::toString(wl);
}

qint64 RerollDiceNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_nextNode)
    {
        priority= m_nextNode->getPriority();
    }

    return priority;
}
ExecutionNode* RerollDiceNode::getCopy() const
{
    RerollDiceNode* node= new RerollDiceNode(m_reroll, m_adding);
    node->setValidatorList(m_validatorList);
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

ExecutionNode* RerollDiceNode::getInstruction() const
{
    return m_instruction;
}

void RerollDiceNode::setInstruction(ExecutionNode* instruction)
{
    m_instruction= instruction;
}
