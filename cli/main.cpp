/***************************************************************************
* Copyright (C) 2014 by Renaud Guezennec                                   *
* http://www.rolisteam.org/contact                      *
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
bool markdown = false;
QString diceToMarkdown(QList<ExportedDiceResult>& diceList,bool highlight,bool homogeneous)
{
    QStringList global;
    for(auto dice : diceList)
    {
        QStringList resultGlobal;
        for(int face:  dice.keys())
        {
            QStringList result;
            QStringList currentStreak;
            QList<QStringList> allStreakList;
            ListDiceResult diceResult =  dice.value(face);
            bool previousHighlight=false;
            QString previousColor;
            QString patternColor("");
            for (const HighLightDice& tmp : diceResult)
            {
                if(previousColor != tmp.getColor())
                {
                    if(!currentStreak.isEmpty())
                    {
                        QStringList list;
                        list << patternColor+currentStreak.join(',');
                        allStreakList.append(list);
                        currentStreak.clear();
                    }
                    patternColor = QStringLiteral("");

                }
                QStringList diceListStr;
                if((previousHighlight)&&(!tmp.isHighlighted()))
                {
                    if(!currentStreak.isEmpty())
                    {
                        QStringList list;
                        list << patternColor+currentStreak.join(',');
                        allStreakList.append(list);
                        currentStreak.clear();
                    }

                }
                else if((!previousHighlight)&&(tmp.isHighlighted()))
                {
                    if(!currentStreak.isEmpty())
                    {
                        QStringList list;
                        list << currentStreak.join(',');
                        allStreakList.append(list);
                        currentStreak.clear();
                    }
                }
                previousHighlight = tmp.isHighlighted();
                previousColor = tmp.getColor();
                for(int i =0; i < tmp.getResult().size(); ++i)
                {
                    qint64 dievalue = tmp.getResult()[i];
                    diceListStr << QString::number(dievalue);
                }
                if(diceListStr.size()>1)
                {
                    QString first = diceListStr.takeFirst();
                    first = QString("%1 [%2]").arg(first).arg(diceListStr.join(','));
                    diceListStr.clear();
                    diceListStr << first;
                }
                currentStreak << diceListStr.join(' ');
            }

            if(previousHighlight)
            {
                QStringList list;
                list <<  patternColor+currentStreak.join(',');
                allStreakList.append(list);
            }
            else
            {
                if(!currentStreak.isEmpty())
                {
                    QStringList list;
                    list << currentStreak.join(',');
                    allStreakList.append(list);
                }
            }
            for(const QStringList& a : allStreakList)
            {
                result << a;
            }
            if(dice.keys().size()>1)
            {
                resultGlobal << QString(" d%2:(%1)").arg(result.join(",")).arg(face);
            }
            else
            {
                resultGlobal << result.join(",");
            }
        }
        global << resultGlobal.join("");
    }
    return global.join(";");
}

QString diceToText(QList<ExportedDiceResult>& diceList,bool highlight,bool homogeneous)
{
    QStringList global;
    for(auto dice : diceList)
    {
        QStringList resultGlobal;
        for(int face : dice.keys())
        {
            QStringList result;
            ListDiceResult diceResult =  dice.value(face);
            for (const HighLightDice& tmp : diceResult)
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
                        if(tmp.getColor()=="red")
                        {
                            prefix = "\e[31m%1\e[0m";
                        }
                        if(tmp.getColor()=="black")
                        {
                            prefix = "\e[30m%1\e[0m";
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
        global << resultGlobal.join(' ');
    }
    return global.join(" ; ");
}
void startDiceParsingMarkdown(QString cmd)
{
    QString result("");
    bool highlight = true;
    DiceParser parser;

    //setAlias
    parser.insertAlias(new DiceAlias("l5r5R","L[-,⨀,⨀⬢,❂⬢,❁,❁⬢]"),0);
    parser.insertAlias(new DiceAlias("l5r5S","L[-,-,⨀,⨀,⨀❁,⨀⬢,⨀⬢,❂,❂⬢,❁,❁,❁]"),1);


    if(parser.parseLine(cmd))
    {
        parser.Start();
        if(!parser.getErrorMap().isEmpty())
        {
            result +=  "```markdown\n# Error:\n" + parser.humanReadableError() + "\n```";
        }
        else
        {
            QList<ExportedDiceResult> list;
            bool homogeneous = true;
            parser.getLastDiceResult(list,homogeneous);
            QString listText = diceToText(list,false,homogeneous);
            QString diceText = diceToMarkdown(list,highlight,homogeneous);
            QString scalarText;
            QString str;

            if(parser.hasIntegerResultNotInFirst())
            {
                auto values = parser.getLastIntegerResults();
                QStringList strLst;
                for(auto val : values )
                {
                    strLst << QString::number(val);
                }
                scalarText = QString("%1").arg(strLst.join(','));
            }
            else if(!list.isEmpty())
            {
                auto values = parser.getSumOfDiceResult();
                QStringList strLst;
                for(auto val : values )
                {
                    strLst << QString::number(val);
                }
                scalarText = QString("%1").arg(strLst.join(','));
            }
            if(highlight)
            {
                str = QString("```markdown\n# %1\nDetails:[%3 (%2)]\n```").arg(scalarText).arg(diceText).arg(parser.getDiceCommand());
            }
            else
            {
                str = QString("```markdown\n#%1, details:[%3 (%2)]\n```").arg(scalarText).arg(diceText).arg(parser.getDiceCommand());
            }
            if(parser.hasStringResult())
            {
                bool ok;
                QStringList allStringlist = parser.getAllStringResult(ok);
                QString stringResult = allStringlist.join(" ; ");
                stringResult.replace("%1",scalarText);
                stringResult.replace("%2",listText.trimmed());
                str = stringResult;
            }
            result += str + "\n";
        }

    }
    else
    {
        result += "markdown\n#Error:" + parser.humanReadableError() + "\n```";
    }

    out << result;
}

void startDiceParsing(QStringList& cmds,QString& treeFile,bool highlight)
{
    DiceParser* parser = new DiceParser();

    for(QString cmd : cmds)
    {

        if(parser->parseLine(cmd))
        {
            //

            parser->Start();
            if(!parser->getErrorMap().isEmpty())
            {
                out << "Error" << parser->humanReadableError() << "\n";
                return;
            }

            QList<ExportedDiceResult> list;
            bool homogeneous = true;
            parser->getLastDiceResult(list,homogeneous);
            QString diceText = diceToText(list,highlight,homogeneous);


            QString scalarText;
            QString str;

            if(parser->hasIntegerResultNotInFirst())
            {
                auto values = parser->getLastIntegerResults();
                QStringList strLst;
                for(auto val : values )
                {
                    strLst << QString::number(val);
                }
                scalarText = QString("%1").arg(strLst.join(','));
            }
            else if(!list.isEmpty())
            {
                auto values = parser->getSumOfDiceResult();
                QStringList strLst;
                for(auto val : values )
                {
                    strLst << QString::number(val);
                }
                scalarText = QString("%1").arg(strLst.join(','));
            }
            if(highlight)
                str = QString("Result: \e[0;31m%1\e[0m, details:[%3 (%2)]").arg(scalarText).arg(diceText).arg(parser->getDiceCommand());
            else
                str = QString("Result: %1, details:[%3 (%2)]").arg(scalarText).arg(diceText).arg(parser->getDiceCommand());

            if(parser->hasStringResult())
            {
                str = parser->getStringResult().join(",");;
            }
            if(!parser->getComment().isEmpty())
            {
                out << "\033[1m" <<parser->getComment() << "\033[0m\n";
            }
            out << str << "\n";
            if(!treeFile.isEmpty())
            {
                parser->writeDownDotTree(treeFile);
            }

        }
        else
        {
            out << parser->humanReadableError() << "\n";;
        }
    }
    delete parser;
}
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QStringList commands;
    QString cmd;
    QString dotFileStr;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    bool colorb=true;
    out.setCodec("UTF-8");// UTF-8 UTF-8
   // std::cout << QString("réussite").toStdString();

    QCommandLineParser optionParser;
    QCommandLineOption color(QStringList() << "c"<< "color-off", "Disable color to highlight result");
    QCommandLineOption version(QStringList() << "v"<< "version", "Show the version and quit.");
    QCommandLineOption reset(QStringList() << "reset-settings", "Erase the settings and use the default parameters");
    QCommandLineOption discord(QStringList() << "m" <<"markdown", "The output is formatted in markdown.");
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
    optionParser.addOption(discord);
    optionParser.addOption(translation);
    optionParser.addOption(help);

    for(int i=0;i<argc;++i)
    {
        commands << QString::fromUtf8(argv[i]);
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
    if(optionParser.isSet(discord))
    {
        markdown = true;
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


    if(markdown)
    {
        startDiceParsingMarkdown(cmdList.first());
    }
    else
    {
        startDiceParsing(cmdList,dotFileStr,colorb);
    }
    if(optionParser.isSet(help))
    {
        out << optionParser.helpText();
    }



    return 0;
}
