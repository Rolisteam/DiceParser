#include "range.h"

Range::Range()
{


}
void Range::setValue(qint64 s,qint64 e)
{
    m_start = s;
    m_end=e;
}

qint64 Range::hasValid(Die* m,bool recursive) const
{
    if(recursive)
    {
        qint64 i = 0;
        foreach(qint64 value, m->getListValue())
        {
            if((value>=m_start)&&(value<=m_end))
            {
                ++i;
            }
        }
        return i;
    }
    else if((m->getLastRolledValue()>=m_start)&&(m->getLastRolledValue()<=m_end))
    {
        return 1;
    }
    return 0;
}
