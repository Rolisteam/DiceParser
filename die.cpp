#include "die.h"

Die::Die()
    : m_hasValue(false)
{
}

void Die::setValue(qint64 r)
{
    m_value = r;
    m_hasValue = true;
}

void Die::insertRollValue(qint64 r)
{
    m_rollResult.append(r);
}

void Die::setSelected(bool b)
{
    m_selected = b;
}


bool Die::isSelected() const
{
    return m_selected;
}
qint64 Die::getValue() const
{
    if(m_hasValue)
        return m_value;
    else
    {
        qint64 value=0;
        foreach(qint64 tmp,m_rollResult)
        {
            value+=tmp;
        }
        return value;
    }
}
QList<qint64> Die::getListValue() const
{
    return m_rollResult;
}
bool Die::hasChildrenValue()
{
    return m_rollResult.size()>1?true:false;
}
