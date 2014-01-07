#include "booleancondition.h"


BooleanCondition::BooleanCondition()
{
}
bool BooleanCondition::isValid(Die b) const
{
    switch(m_operator)
    {
    case Equal:
        return (b.getValue()==m_value);
    case GreaterThan:
        return (b.getValue()>m_value);
    case LesserThan:
        return (b.getValue()<m_value);
    case GreaterOrEqual:
        return (b.getValue()>=m_value);
    case LesserOrEqual:
        return (b.getValue()<=m_value);


    }
    return false;
}

void BooleanCondition::setOperator(LogicOperator m)
{
    m_operator = m;
}

void BooleanCondition::setValue(qint64 v)
{
    m_value=v;
}
