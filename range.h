#ifndef RANGE_H
#define RANGE_H

#include <Qt>
#include "validator.h"

class Range : public Validator
{
public:
    Range();
    void setValue(qint64,qint64);

    virtual bool isValid(Die* b) const;

private:
    qint64 m_start;
    qint64 m_end;
};

#endif // RANGE_H
