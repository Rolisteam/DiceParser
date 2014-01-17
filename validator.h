#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <Qt>
#include "die.h"

class Validator
{
public:
    Validator();
    virtual qint64 hasValid(Die* b,bool recursive) const = 0 ;
};

#endif // VALIDATOR_H
