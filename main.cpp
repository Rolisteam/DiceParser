#include <QCoreApplication>

#include "diceparser.h"

int main(int argc, char *argv[])
{
    // QCoreApplication a(argc, argv);
    DiceParser* myParser = new DiceParser();

    myParser->parseLine("3D100");
    myParser->parseLine("3D100");
    //myParser->parseLine("100291D66666666");
    //myParser->parseLine("10D10g3");
    //myParser->parseLine("10g3");
    myParser->parseLine("1D8");
    myParser->parseLine("1D8+2D6+7");
    myParser->parseLine("D25");
    myParser->parseLine("8+8");
    myParser->parseLine("8-88");
    myParser->parseLine("100*28");
    myParser->parseLine("100/28");
    myParser->parseLine("100/8");
    myParser->parseLine("100*3");
    //return a.exec();
    return 0;
}
