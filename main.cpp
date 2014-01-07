#include <QCoreApplication>

#include "diceparser.h"

int main(int argc, char *argv[])
{
    // QCoreApplication a(argc, argv);
    DiceParser* myParser = new DiceParser();

    myParser->parseLine("3D100");
    myParser->parseLine("3D100");
   // myParser->parseLine("100291D66666666");
    myParser->parseLine("10D10K3");
    //myParser->parseLine("10k3");

    /// roll 10 dice of 10 slices and sort them
    myParser->parseLine("10D10s");

    /// roll 15 dice of 10 slices and count above 7
    myParser->parseLine("15D10c[8-10]");

    /// roll 15 dice of 10 slices condition count above 7
    myParser->parseLine("15D10c[>7]");


    myParser->parseLine("1D8+2D6+7");
    myParser->parseLine("D25");
    myParser->parseLine("8+8");
    myParser->parseLine("88-1D20");
    myParser->parseLine("100*1D20*2D6");
    myParser->parseLine("100/28");
    myParser->parseLine("100/8");
    myParser->parseLine("100*3");
    //return a.exec();
    return 0;
}
