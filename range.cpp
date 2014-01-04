#include "range.h"

Range::Range()
{


}
void Range::setValue(qint64 s,qint64 e)
{
    m_start = s;
    m_end=e;
}

bool Range::isValid(qint64 m) const
{
    if((m>=m_start)&&(m<=m_end))
    {
        return true;
    }
    return false;
}
