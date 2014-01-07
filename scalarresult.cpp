#include "scalarresult.h"

ScalarResult::ScalarResult()
{
}


void ScalarResult::setValue(qint64 i)
{
    m_value=i;
}
qint64 ScalarResult::getScalar()
{
    return m_value;
}

bool ScalarResult::isScalar() const
{
    return true;
}
