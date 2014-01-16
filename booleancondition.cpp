#include "booleancondition.h"


BooleanCondition::BooleanCondition()
{
}
qint64 BooleanCondition::hasValid(Die* b,bool recursive) const
{
    QList<qint64> listValues;
    if(recursive)
    {
        listValues = b->getListValue();
    }
    else
    {
        listValues.append(b->getLastRolledValue());
    }

    qint64 sum= 0;
    foreach(qint64 value, listValues)
    {

        switch(m_operator)
        {
            case Equal:
                sum+=(value==m_value)?1:0;
                break;
            case GreaterThan:
                sum+= (value>m_value)?1:0;
                break;
            case LesserThan:
                sum+= (value<m_value)?1:0;
                break;
            case GreaterOrEqual:
                sum+= (value>=m_value)?1:0;
                break;
            case LesserOrEqual:
                sum+= (value<=m_value)?1:0;
                break;


        }
    }

    return sum;
}

void BooleanCondition::setOperator(LogicOperator m)
{
    m_operator = m;
}

void BooleanCondition::setValue(qint64 v)
{
    m_value=v;
}
