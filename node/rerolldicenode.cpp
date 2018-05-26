#include "rerolldicenode.h"
#include "parsingtoolbox.h"

RerollDiceNode::RerollDiceNode(bool reroll, bool addingMode)
    : m_diceResult(new DiceResult())
    , m_validator(nullptr)
    , m_reroll(reroll)
    , m_adding(addingMode)
{
    m_result=m_diceResult;
}
RerollDiceNode::~RerollDiceNode()
{
	if(nullptr!=m_validator)
	{
		delete m_validator;
		m_validator = nullptr;
	}
}
void RerollDiceNode::run(ExecutionNode* previous)
{
    m_previousNode = previous;
    if((nullptr!=previous)&&(nullptr!=previous->getResult()))
    {
        DiceResult* previous_result = dynamic_cast<DiceResult*>(previous->getResult());
        m_result->setPrevious(previous_result);
        if(nullptr!=previous_result)
        {
            for(Die* die: previous_result->getResultList())
            {
                Die* tmpdie = new Die();
                *tmpdie=*die;
                m_diceResult->insertResult(tmpdie);
                die->displayed();
            }
            //m_diceResult->setResultList(list);

            QList<Die*>& list = m_diceResult->getResultList();
            QList<Die*> toRemove;

            for(int i = 0; i < list.size() ; ++i)
            {
                auto die = list.at(i);
                while(m_validator->hasValid(die,false))
                {
                    die->roll(m_adding);
                    if(m_reroll)
                    {
                        break;
                    }
                }
            }

            for(auto die: toRemove)
            {
               list.removeOne(die);      
            }
            

            if(nullptr!=m_nextNode)
            {
                m_nextNode->run(this);
            }
        }
        else
        {
            m_errors.insert(ExecutionNode::DIE_RESULT_EXPECTED,
                            QObject::tr(" The a operator expects dice result. Please check the documentation and fix your command."));
        }
    }
}
void RerollDiceNode::setValidator(Validator* val)
{
      m_validator = val;
}
QString RerollDiceNode::toString(bool wl) const
{
	if(wl)
	{
		return QString("%1 [label=\"RerollDiceNode validatior: %2\"]").arg(m_id).arg(m_validator->toString());
	}
	else
	{
		return m_id;
	}
	//return QString("RerollDiceNode [label=\"RerollDiceNode validatior:%1\"");
}
qint64 RerollDiceNode::getPriority() const
{
    qint64 priority=0;
    if(nullptr!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
ExecutionNode* RerollDiceNode::getCopy() const
{
    RerollDiceNode* node = new RerollDiceNode(m_reroll, m_adding);
    node->setValidator(m_validator);
    if(nullptr!=m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}

ExecutionNode *RerollDiceNode::getInstruction() const
{
    return m_instruction;
}

void RerollDiceNode::setInstruction(ExecutionNode *instruction)
{
    m_instruction = instruction;
}
