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
#include <QTextStream>
#include "highlightdice.h"

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



QTextStream out(stdout, QIODevice::WriteOnly);

QString diceToText(ExportedDiceResult& dice,bool highlight,bool homogeneous)
{
    QStringList resultGlobal;
        foreach(int face, dice.keys())
        {
               QStringList result;
               ListDiceResult diceResult =  dice.value(face);
               //patternColor = patternColorarg();
               foreach (HighLightDice tmp, diceResult)
               {
                    QStringList diceListStr;
                    QStringList diceListChildren;


                    for(int i =0; i < tmp.getResult().size(); ++i)
                    {
                        qint64 dievalue = tmp.getResult()[i];
                        QString prefix("%1");

                        if((tmp.isHighlighted())&&(highlight))
                        {
                            if(tmp.getColor().isEmpty()|| tmp.getColor()=="black")
                            {
                                prefix = "\e[0;31m%1\e[0m";
                            }
                            if(tmp.getColor()=="white")
                            {
                                prefix = "\e[97m%1\e[0m";
                            }
                            if(tmp.getColor()=="blue")
                            {
                                prefix = "\e[34m%1\e[0m";
                            }
                        }

                        if(i==0)
                        {
                            diceListStr << prefix.arg(QString::number(dievalue));
                        }
                        else
                        {
                            diceListChildren << prefix.arg(QString::number(dievalue));
                        }
                    }
                    if(!diceListChildren.isEmpty())
                    {
                        diceListStr << QString("[%1]").arg(diceListChildren.join(' '));
                    }

                    result << diceListStr.join(' ');
                   // qDebug() << result << tmp.first << tmp.second;
               }

               if(dice.keys().size()>1)
               {
                  resultGlobal << QString(" d%2:(%1)").arg(result.join(',')).arg(face);
               }
               else
               {
                   resultGlobal << result;
               }
        }
    return resultGlobal.join(' ');
}

void startDiceParsing(QString& cmd,QString& treeFile,bool highlight)
{
    DiceParser* parser = new DiceParser();

    if(parser->parseLine(cmd))
    {
       //
        if(treeFile.isEmpty())
        {
            parser->Start();
            if(!parser->getErrorMap().isEmpty())
            {
                out << "Error" << parser->humanReadableError()<< "\n";
                return;
            }

            ExportedDiceResult list;
            bool homogeneous = true;
            parser->getLastDiceResult(list,homogeneous);
            QString diceText = diceToText(list,highlight,homogeneous);
            QString scalarText;
            QString str;

            if(parser->hasIntegerResultNotInFirst())
            {
                scalarText = QString("%1").arg(parser->getLastIntegerResult());
            }
            else if(!list.isEmpty())
            {
                scalarText = QString("%1").arg(parser->getSumOfDiceResult());
            }
            if(highlight)
                str = QString("Result: \e[0;31m%1\e[0m, details:[%3 (%2)]").arg(scalarText).arg(diceText).arg(parser->getDiceCommand());
            else
                str = QString("Result: %1, details:[%3 (%2)]").arg(scalarText).arg(diceText).arg(parser->getDiceCommand());

            if(parser->hasStringResult())
            {
                str = parser->getStringResult();
            }
            out << str << "\n";
        }
        else
        {
            parser->writeDownDotTree(treeFile);
        }
    }
    else
    {
        out << parser->humanReadableError()<< "\n";;
    }
}

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
    /*QCoreApplication app(argc,argv);
    QCoreApplication::setApplicationName("dice");
    QCoreApplication::setApplicationVersion("1.0");*/


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
}
