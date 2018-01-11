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
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>

#include "diceparser.h"
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



QJsonArray diceToJson(QList<ExportedDiceResult>& diceList,bool& highlight,bool& homogeneous)
{
    QJsonArray array;
    for(auto dice : diceList)
    {
        for(int face:  dice.keys())
        {
            QStringList result;
            QJsonObject diceList;
            ListDiceResult diceResults =  dice.value(face);
            std::vector<std::vector<HighLightDice>> sameColorDice;
            std::vector<QString> alreadyDoneColor;
            for(auto & dice : diceResults)
            {
                auto it=std::find_if(alreadyDoneColor.begin(), alreadyDoneColor.end(),[dice](QString color){
                    return color == dice.getColor();
                });

                if( it == alreadyDoneColor.end())
                {
                   sameColorDice.push_back(std::vector<HighLightDice>());
                   alreadyDoneColor.push_back(dice.getColor());
                   it = alreadyDoneColor.end();
                   --it;
                }
                
                int i = std::distance(alreadyDoneColor.begin(), it);
                qDebug() << "index:" <<i << sameColorDice.size();
                sameColorDice[i].push_back(dice);
            }
            int i = 0;
            for(auto it = alreadyDoneColor.begin() ; it != alreadyDoneColor.end(); ++it)
            {
                auto list = sameColorDice[i];
              	QJsonObject object;
                object["color"]=*it;
                object["face"]=face;
                QJsonArray values;
                for(auto const dice : list)
                {
                    for(auto result : dice.getResult())
                    {
                        values.push_back((qint64)result);
                    }
                }
                object["values"]=values;
                ++i;
                array.push_back(object);
            } 
        }
    }
    return array;
}

QString diceToMarkdown(QList<ExportedDiceResult>& diceList,bool highlight,bool homogeneous)
{
    auto array = diceToJson(diceList,highlight,homogeneous);
    qDebug() << array;
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
            QString previousColor("");
            QString patternColor("");
            for (const HighLightDice& tmp : diceResult)
            {
                if(previousColor != tmp.getColor())
                {
                    homogeneous = false;
                    if(!currentStreak.isEmpty())
                    {
                        QStringList list;
                        list << patternColor+currentStreak.join(',')+"</tspan>";
                        allStreakList.append(list);
                        currentStreak.clear();
                    }
                    QString color = tmp.getColor();
                    if(color.isEmpty())
                        color = "black";
                    patternColor = QStringLiteral("<tspan fill=\"%1\">").arg(color);

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
                QString end="</tspan>";
                if(patternColor.isEmpty())
                    end=patternColor;
                list <<  patternColor+currentStreak.join(',')+end;
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
        foreach(int face, dice.keys())
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
                        if(tmp.getColor()=="green")
                        {
                            prefix = "\e[32m%1\e[0m";
                        }
                        if(tmp.getColor()=="yellow")
                        {
                            prefix = "\e[33m%1\e[0m";
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
    /*QHash<QString, QString>* vars = new QHash<QString,QString>();
    vars->insert("adresse","4");
    vars->insert("competence arme","7");
    parser.setVariableDictionary(vars);*/
    //setAlias
    parser.insertAlias(new DiceAlias("L5R5R","L[-,⨀,⨀⬢,❂⬢,❁,❁⬢]"),0);
    parser.insertAlias(new DiceAlias("L5R5S","L[-,-,⨀,⨀,⨀❁,⨀⬢,⨀⬢,❂,❂⬢,❁,❁,❁]"),1);


    if(parser.parseLine(cmd))
    {
        parser.start();
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
            if(homogeneous)
            {
                if(highlight)
                {
                    str = QString("```markdown\n# %1\nDetails:[%3 (%2)]\n```").arg(scalarText).arg(diceText).arg(parser.getDiceCommand());
                }
                else
                {
                    str = QString("```markdown\n#%1, details:[%3 (%2)]\n```").arg(scalarText).arg(diceText).arg(parser.getDiceCommand());
                }
            }
            else
            {
                str = QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<svg version=\"1.1\"  xmlns=\"http://www.w3.org/2000/svg\" "
                              "xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n"
                              "<text font-size=\"16\" x=\"0\" y=\"20\">\n"
                                  "<tspan fill=\"red\">%1</tspan> details:[%3 (%2)]"
                                "</text>\n"
                              "</svg>\n").arg(scalarText).arg(diceText).arg(parser.getDiceCommand());
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
            if(!parser.getComment().isEmpty())
            {
                str.prepend(parser.getComment()+ QStringLiteral("\n"));
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

            parser->start();
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
                bool ok;
                QStringList allStringlist = parser->getAllStringResult(ok);
                QString stringResult = allStringlist.join(" ; ");
                stringResult.replace("%1",scalarText);
                stringResult.replace("%2",diceText.trimmed());
                str = stringResult;       
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
    QCommandLineOption alias(QStringList() << "a" << "alias", "path to alias json files: <aliasfile>","aliasfile");
    QCommandLineOption character(QStringList() << "s" << "charactersheet", "set Parameters to simulate character sheet: <sheetfile>","sheetfile");
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
    optionParser.addOption(alias);
    optionParser.addOption(character);
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

    QString aliasstr;
    if(optionParser.isSet(alias))
    {
        aliasstr = optionParser.value(alias);
    }
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
