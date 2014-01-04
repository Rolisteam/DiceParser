#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <Qt>

class Validator
{
public:
    Validator();
    virtual bool isValid(qint64 b) const = 0 ;
};

#endif // VALIDATOR_H
