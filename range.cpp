#include "range.h"

Range::Range()
{


}
void Range::setValue(qint64 s,qint64 e)
{
    m_start = s;
    m_end=e;
}

bool Range::isValid(Die* m) const
{
    if((m->getLastRolledValue()>=m_start)&&(m->getLastRolledValue()<=m_end))
    {
        return true;
    }
    return false;
}
