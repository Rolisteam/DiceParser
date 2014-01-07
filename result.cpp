#include "result.h"

Result::Result()
{
}

Result* Result::getPrevious()
{
    return m_previous;
}

void Result::setPrevious(Result* p)
{
    m_previous = p;
}

