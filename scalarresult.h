#ifndef SCALARRESULT_H
#define SCALARRESULT_H

#include "result.h"
#include <Qt>


class ScalarResult : public Result
{
public:
    ScalarResult();

    virtual bool isScalar() const;
    virtual qint64 getScalar();

    void setValue(qint64 i);

private:
    qint64 m_value;
};

#endif // SCALARRESULT_H
