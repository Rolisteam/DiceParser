#include "result.h"

Result::Result()
    : m_previous(NULL)
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

