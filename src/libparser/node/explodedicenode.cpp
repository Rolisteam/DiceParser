#include "explodedicenode.h"
#include "booleancondition.h"
#include "diceparser/parsingtoolbox.h"
#include "validator.h"
#include "validatorlist.h"

ExplodeDiceNode::ExplodeDiceNode() : m_diceResult(new DiceResult())
{
    m_result= m_diceResult;
}
void ExplodeDiceNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    if(!previous)
        return;

    if(!previous->getResult())
        return;

    DiceResult* previous_result= dynamic_cast<DiceResult*>(previous->getResult());
    m_result->setPrevious(previous_result);

    if(!previous_result)
        return;

    auto diceList= previous_result->getResultList();

    auto allInvalid= std::all_of(std::begin(diceList), std::end(diceList),
                                 [](const Die* die) { return die->getMaxValue() < die->getBase(); });

    qint64 maxVal= 0;
    if(allInvalid)
    {

        auto const& list= m_validatorList->validators();
        auto max= std::max_element(std::begin(list), std::end(list),
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

    for(auto& die : diceList)
    {
        Die* tmpdie= new Die(*die);
        m_diceResult->insertResult(tmpdie);
        die->displayed();
        if(allInvalid && maxVal != tmpdie->getMaxValue() && maxVal > tmpdie->getBase())
        {
            qInfo() << "Invalid range for explosing dice, set " << maxVal << " as maximum" << tmpdie->getMaxValue();
            tmpdie->setMaxValue(maxVal);
        }
    }

    qint64 limit= -1;
    if(m_limit)
    {
        m_limit->execute(this);
        auto limitNode= ParsingToolBox::getLeafNode(m_limit);
        auto result= limitNode->getResult();
        if(result->hasResultOfType(Dice::RESULT_TYPE::SCALAR))
            limit= static_cast<quint64>(result->getResult(Dice::RESULT_TYPE::SCALAR).toInt());
    }

    bool hasExploded= false;
    std::function<void(Die*, qint64)> f= [&hasExploded, this, limit](Die* die, qint64)
    {
        static QHash<Die*, qint64> explodePerDice;
        auto validity
            = m_validatorList->isValidRangeSize(std::make_pair<qint64, qint64>(die->getBase(), die->getMaxValue()));
        isValid(Dice::CONDITION_STATE::ALWAYSTRUE == validity, Dice::ERROR_CODE::ENDLESS_LOOP_ERROR,
                QObject::tr("Condition (%1) cause an endless loop with this dice: %2")
                    .arg(toString(true))
                    .arg(QStringLiteral("d[%1,%2]")
                             .arg(static_cast<int>(die->getBase()))
                             .arg(static_cast<int>(die->getMaxValue()))));

        isValid(die->getBase() > die->getMaxValue(), Dice::ERROR_CODE::ENDLESS_LOOP_ERROR,
                QObject::tr("Condition (%1) cause an endless loop with dice: %2")
                    .arg(toString(true))
                    .arg(QStringLiteral("d[%1,%2]")
                             .arg(static_cast<int>(die->getBase()))
                             .arg(static_cast<int>(die->getMaxValue()))));

        hasExploded= true;
        if(limit >= 0)
        {
            auto& d= explodePerDice[die];
            if(d == limit)
            {
                hasExploded= false;
                return;
            }
            ++d;
        }
        die->roll(true);
    };
    do
    {
        hasExploded= false;
        m_validatorList->validResult(m_diceResult, false, false, f);
    } while(hasExploded);
}
ExplodeDiceNode::~ExplodeDiceNode()
{
    if(nullptr != m_validatorList)
    {
        delete m_validatorList;
    }
}
void ExplodeDiceNode::setValidatorList(ValidatorList* val)
{
    m_validatorList= val;
}
QString ExplodeDiceNode::toString(bool withlabel) const
{
    return withlabel ? QString("%1 [label=\"ExplodeDiceNode %2\"]").arg(m_id, m_validatorList->toString()) :
                       ExecutionNode::toString(withlabel);
}
qint64 ExplodeDiceNode::getPriority() const
{
    qint64 priority= 0;
    if(nullptr != m_previousNode)
    {
        priority= m_previousNode->getPriority();
    }
    return priority;
}

ExecutionNode* ExplodeDiceNode::getCopy() const
{
    ExplodeDiceNode* node= new ExplodeDiceNode();
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

void ExplodeDiceNode::setLimitNode(ExecutionNode* limitNode)
{
    m_limit= limitNode;
}
