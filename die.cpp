#include "die.h"

Die::Die()
{
}

void Die::setValue(qint64 r)
{
    m_value = r;
}

void Die::insertRollValue(qint64 r)
{
    m_rollResult.insert(r);
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
    return m_value;
}
QList<qint64> Die::getListValue() const
{
    return m_rollResult;
}
