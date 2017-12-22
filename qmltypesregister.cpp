
#include "qmltypesregister.h"

#include <QQmlEngine>
#include "diceroller.h"


void registerQmlTypes()
{
    qmlRegisterType<DiceRoller>("Dice", 1, 0, "DiceRoller");
}
