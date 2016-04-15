/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://www.rolisteam.org/contact/                                        *
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
#include "botircdiceparser.h"
/**
 * @page Dice
 * The cli for DiceParser the new dice system from rolisteam.
 * @section Build and install
 * To build this program, type these command:
 * - mkdir build
 * - cd build
 * - cmake ../
 * - make
 * - make install
 * @return
 */
int main(int argc, char *argv[])
{
    QCoreApplication app(argc,argv);
    BotIrcDiceParser bot;
    bot.connectToServer();
    return    app.exec();
}


/*QTextStream out(stdout, QIODevice::WriteOnly);



void usage()
{
    QString help = "Usage: ./dice [options]\n\
\n\
Options:\n\
  -c, --color-off                      Disable color to highlight result\n\
  -v, --version                        Show the version and quit.\n\
  --reset-settings                     Erase the settings and use the default\n\
                                       parameters\n\
  -d, --dot-file <dotfile>             Instead of rolling dice, generate the\n\
                                       execution tree and write it in <dotfile>\n\
  -t, --translation <translationfile>  path to the translation file:\n\
                                       <translationfile>\n\
  -h, --help                           Display this help\n\
";

    out << help;
}


int main(int argc, char *argv[])
{



    QStringList commands;
    QString cmd;
    QString dotFileStr;

    bool colorb=true;

    QCommandLineParser optionParser;
    QCommandLineOption color(QStringList() << "c"<< "color-off", "Disable color to highlight result");
    QCommandLineOption version(QStringList() << "v"<< "version", "Show the version and quit.");
    QCommandLineOption reset(QStringList() << "reset-settings", "Erase the settings and use the default parameters");
    QCommandLineOption dotFile(QStringList() << "d"<<"dot-file", "Instead of rolling dice, generate the execution tree and write it in <dotfile>","dotfile");
    QCommandLineOption translation(QStringList() << "t"<<"translation", "path to the translation file: <translationfile>","translationfile");
    QCommandLineOption help(QStringList() << "h"<<"help", "Display this help");

     if(!optionParser.addOption(color))
     {
        out << optionParser.errorText() << "\n";
     }

    optionParser.addOption(version);
    optionParser.addOption(reset);
    optionParser.addOption(dotFile);
    optionParser.addOption(translation);
    optionParser.addOption(help);

    for(int i=0;i<argc;++i)
    {

        commands << QString::fromLatin1(argv[i]);
    }

    optionParser.process(commands);




    if(optionParser.isSet(color))
    {
        commands.removeAt(0);
        colorb = false;
    }
    else if(optionParser.isSet(version))
    {
        out << "Rolisteam DiceParser v1.0.0"<< "\n";
        out << "More Details: www.rolisteam.org"<< "\n";
        return 0;
    }
    else if(optionParser.isSet(reset))
    {
        return 0;
    }
    else if(optionParser.isSet(dotFile))
    {
         dotFileStr = optionParser.value(dotFile);
    }
    else if(optionParser.isSet(translation))
    {

    }
    else if(optionParser.isSet(help))
    {
        cmd = "help";
    }
    QStringList cmdList = optionParser.positionalArguments();
   // qDebug()<< "rest"<< cmdList;

    if(!cmdList.isEmpty())
    {
        cmd = cmdList.first();
    }
    if(!cmd.isEmpty())
    {
        startDiceParsing(cmd,dotFileStr,colorb);
        if(cmd=="help")
        {
            usage();
        }
    }

    return 0;
}*/
