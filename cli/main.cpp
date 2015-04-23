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

#include <QStringList>
#include "diceparser.h"
#include <QCommandLineParser>
#include <QCommandLineOption>

void startDiceParsing(QString& cmd,QString& treeFile,bool highlight)
{
    DiceParser* parser = new DiceParser();
    if(parser->parseLine(cmd))
    {
        parser->Start();
       //
        if(treeFile.isEmpty())
        {
            parser->displayResult();
        }
        else
        {
            parser->writeDownDotTree();
        }
    }
}




int main(int argc, char *argv[])
{

    QStringList commands;
    QString cmd;
    QString dotFile;

    bool color=true;

    QCommandLineParser optionParser;
    QCommandLineOption color(QStringList() << "co"<< "color-off", tr("Disable color to highlight result"));
    QCommandLineOption version(QStringList() << "v"<< "version", tr("Show the version and quit."));
    QCommandLineOption reset(QStringList() << "reset-settings", tr("Erase the settings and use the default parameters"));
    QCommandLineOption dotFile(QStringList() << "d"<<"dot-file", tr("Instead of rolling dice, generate the execution tree and write it in <dotfile>"),"dotfile");
    QCommandLineOption translation(QStringList() << "t"<<"translation", QObject::tr("path to the translation file: <translationfile>"),"translationfile");
    QCommandLineOption help(QStringList() << "h"<<"help", QObject::tr("Display this help"));

    optionParser.addOption(color);
    optionParser.addOption(version);
    optionParser.addOption(reset);
    optionParser.addOption(dotFile);
    optionParser.addOption(translation);
    optionParser.addOption(help);

    for(int i=1;i<argc;++i)
    {

        commands << QString::fromLatin1(argv[i]);
    }

    optionParser.parse(commands);


    if(optionParser.isSet(color))
    {
        return 0;
    }
    else if(optionParser.isSet(version))
    {
        return 0;
    }
    else if(optionParser.isSet(reset))
    {
        return 0;
    }
    else if(optionParser.isSet(dotFile))
    {
         dotFile = optionParser.value(dotFile);
    }
    else if(optionParser.isSet(translation))
    {

    }
    else if(optionParser.isSet(help))
    {

        cmd = "help";
    }



    cmd = commands.first();


    startDiceParsing(cmd,dotFile,color);

    /*commands<< "10d10c[>6]+@c[=10]"
			<< "1L[cheminée,chocolat,épée,arc,chute de pierre]"
			<< "10d10c[>=6]-@c[=1]"
             << "10d10c[>=6]-@c[=1]-@c[=1]"
             << "10d10c[>6]+@c[=10]"
             << "1+1D10"
             << "3d10c[>=5]"
            << "3nwod"
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
			<< "10d10e11"
            << "1D8+2D6+7"
			<< "100190D6666666s"
            << "D25"
			<< "D25+D10"
			<< "D25;D10"
            << "8+8+8"
            << "1D20-88"
            << "100*1D20*2D6"
            << "100/28*3"
            << "100/8"
            << "100*3*8"
            << "help"
			<< "la"
			<< "400000D20/400000"
			<< "100*3*8";//
*/



	return 0;
}
