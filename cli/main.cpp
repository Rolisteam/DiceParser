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
#include <QDebug>

QString diceToText(ExportedDiceResult& dice)
{
    QStringList resultGlobal;
    foreach(int face, dice.keys())
    {
        QStringList result;
           ListDiceResult diceResult =  dice.value(face);
           bool previousHighlight=false;
           QString patternColor("\e[0;31m");
           //patternColor = patternColorarg();
           foreach (DiceAndHighlight tmp, diceResult)
           {
                QStringList diceListStr;
                QStringList diceListChildren;
                 if((previousHighlight)&&(!tmp.second))
                {
                    result << patternColor << result.join(',') <<"\e[0m";
                }
                 previousHighlight = tmp.second;
                for(int i =0; i < tmp.first.size(); ++i)
                {
                    quint64 dievalue = tmp.first[i];
                    if(i==0)
                    {
                        diceListStr << QString::number(dievalue);
                    }
                    else
                    {
                        diceListChildren << QString::number(dievalue);
                    }
                }
                if(!diceListChildren.isEmpty())
                {
                    diceListStr << QString("[%1]").arg(diceListChildren.join(','));
                }

                result << diceListStr.join(' ');
           }
           if(previousHighlight)
           {
               QStringList list;
               list << patternColor << result.join(',') << "\e[0m";
               result = list;
           }
           if(dice.keys().size()>1)
           {
              resultGlobal << QString(" d%2:(%1)").arg(result.join(' ')).arg(face);
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
        parser->Start();
       //
        if(treeFile.isEmpty())
        {
            ExportedDiceResult list;
            parser->getLastDiceResult(list);
            QString diceText = diceToText(list);
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

            str = QString("Result: \e[0;31m%1\e[0m, details:[%3 (%2)]").arg(scalarText).arg(diceText).arg(parser->getDiceCommand());

            if(parser->hasStringResult())
            {
                str = parser->getStringResult().replace("\n","<br/>");
            }
            qDebug() << str;

        }
        else
        {
            parser->writeDownDotTree(treeFile);
        }
    }
}




int main(int argc, char *argv[])
{

    QStringList commands;
    QString cmd;
    QString dotFileStr;

    bool colorb=false;

    QCommandLineParser optionParser;
    QCommandLineOption color(QStringList() << "co"<< "color-off", "Disable color to highlight result");
    QCommandLineOption version(QStringList() << "v"<< "version", "Show the version and quit.");
    QCommandLineOption reset(QStringList() << "reset-settings", "Erase the settings and use the default parameters");
    QCommandLineOption dotFile(QStringList() << "d"<<"dot-file", "Instead of rolling dice, generate the execution tree and write it in <dotfile>","dotfile");
    QCommandLineOption translation(QStringList() << "t"<<"translation", "path to the translation file: <translationfile>","translationfile");
    QCommandLineOption help(QStringList() << "h"<<"help", "Display this help");

     if(!optionParser.addOption(color))
     {
         qDebug()<< optionParser.errorText();
     }

    optionParser.addOption(version);
    optionParser.addOption(reset);
    optionParser.addOption(dotFile);
    optionParser.addOption(translation);
    optionParser.addOption(help);

    for(int i=1;i<argc;++i)
    {

        commands << QString::fromLatin1(argv[i]);
    }

    if(!optionParser.parse(commands))
    {
        qDebug()<< optionParser.errorText();
    }
    else
         {
        qDebug() << "no error";
    }


    if(optionParser.isSet(color))
    {
        qDebug() << "color";
        colorb = false;
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
         dotFileStr = optionParser.value(dotFile);
    }
    else if(optionParser.isSet(translation))
    {

    }
    else if(optionParser.isSet(help))
    {
        cmd = "help";
    }

    cmd = commands.first();

    qDebug() << "super 5" << cmd << dotFileStr << colorb;
    startDiceParsing(cmd,dotFileStr,colorb);

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
