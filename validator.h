#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <Qt>
#include "die.h"

class Validator
{
public:
    Validator();
    virtual bool isValid(Die* b) const = 0 ;
};

#endif // VALIDATOR_H
