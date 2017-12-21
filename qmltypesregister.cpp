
#include "qmltypesregister.h"

#include "diceroller.h"


void registerQmlTypes()
{

    qmlRegisterType<DiceRoller>("Dice", 1, 0, "DiceRoller");



}
