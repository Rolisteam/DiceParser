#ifndef RESULT_H
#define RESULT_H

#include <Qt>

class Result
{
public:
    Result();

    virtual bool isScalar() const = 0;
    virtual qint64 getScalar() = 0;

    virtual Result* getPrevious();
    virtual void setPrevious(Result*);

private:
    Result* m_previous;
};

#endif // RESULT_H
