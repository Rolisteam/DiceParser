/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://renaudguezennec.homelinux.org/accueil,3.html                      *
*                                                                          *
*  This file is part of DiceParser                                         *
*                                                                          *
* DiceParser is free software; you can redistribute it and/or modify       *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation; either version 2 of the License, or        *
* (at your option) any later version.                                      *
*                                                                          *
* This program is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with this program; if not, write to the                            *
* Free Software Foundation, Inc.,                                          *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 *
***************************************************************************/
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
              <<"(4D6)D10"
           << "1D100a[>=95]a[>=96]a[>=97]a[>=98]a[>=99]e[>=100]"
             << "3D100"
             << "4k3"
             << "10D10e[>=6]sc[>=6]"
             << "100190D6666666s"
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

    if(argc>1)
    {
        for(int i=1;i<argc;++i)
        {

            commands << QString::fromLatin1(argv[i]);
        }
    }


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
