#include <QCoreApplication>


#include <QStringList>
#include "diceparser.h"

int main(int argc, char *argv[])
{
    // QCoreApplication a(argc, argv);
    DiceParser* myParser = new DiceParser();

    QStringList commands;

    commands << "2*(4+4)"
             << "1+(4*3)D10"
             << "2+4/4"
            << "2D10*2D20*8"
            <<"1+(4*3)D10"
           << "1D100a[>=95]a[>=96]a[>=97]a[>=98]a[>=99]e[>=100]"
             << "3D100"
             << "3k3"
             << "10D10e[>=6]s"
             //<< "100190D6666666s"
             << "10D10e10s"
            << "10D10s"
             << "15D10e10c[8-10]"
             << "15D10c[>7]"
             << "1D8+2D6+7"
             << "D25"
             << "8+8+8"
             << "1D20-88"
             << "100*1D20*2D6"
             << "100/28*3"
             << "100/8"
             << "100*3*8"
             << "100*3*8";



    foreach(QString cmd, commands)
    {
        if(myParser->parseLine(cmd))
        {
            myParser->Start();
            myParser->displayResult();
        }
    }

    //return a.exec();
    return 0;
}
