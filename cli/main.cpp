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

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>

#ifdef PAINTER_OP
#include <QGuiApplication>
#else
#include <QCoreApplication>
#endif

#include "dicealias.h"
#include "diceparser.h"
#include "displaytoolbox.h"
#include "highlightdice.h"
#include "parsingtoolbox.h"

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
QTextStream err(stderr, QIODevice::WriteOnly);
bool markdown= false;
#ifdef PAINTER_OP
enum EXPORTFORMAT
{
    TERMINAL,
    SVG,
    IMAGE,
    MARKDOWN,
    JSON,
    BOT,
    TEXT
};
#else
enum EXPORTFORMAT
{
    TERMINAL,
    SVG,
    MARKDOWN,
    JSON,
    BOT,
    TEXT
};
#endif
int returnValue= 0;

void displayJSon(QString json)
{
    out << json << "\n";
}
void displayMarkdown(QString json)
{
    QJsonDocument doc= QJsonDocument::fromJson(json.toUtf8());
    auto obj= doc.object();
    auto error= obj["error"].toString();
    auto warning= obj["warning"].toString();
    auto comment= obj["comment"].toString();
    auto arrayInst= obj["instructions"].toArray();
    QStringList diceResults;
    for(auto inst : arrayInst)
    {
        auto obj= inst.toObject();
        auto diceVals= obj["diceval"].toArray();
        for(auto diceval : diceVals)
        {
            auto objval= diceval.toObject();
            auto resultStr= QString::number(objval["value"].toDouble());

            auto subvalues= objval["subvalues"].toArray();
            QStringList subValueStr;
            for(auto sub : subvalues)
            {
                subValueStr << QString::number(sub.toDouble());
            }
            diceResults << resultStr;
            if(!subValueStr.isEmpty())
                diceResults << QString("[%1]").arg(subValueStr.join(" "));
        }
    }
    auto diceList= diceResults.join(" ");
    auto resultStr= obj["string"].toString();
    auto scalarText= obj["scalar"].toString();
    auto cmd= obj["command"].toString();

    QString str("```Markdown\n");
    if(!error.isEmpty())
    {
        str.append(QStringLiteral("Error: %1\n").arg(error));
    }
    else
    {
        if(!warning.isEmpty())
            str.append(QStringLiteral("Warning: %1\n").arg(warning));

        if(!comment.isEmpty())
        {
            str.prepend(QStringLiteral("%1\n").arg(comment));
        }
        if(!resultStr.isEmpty() && resultStr != scalarText)
        {
            // resultStr.replace("%2", diceList.trimmed());
            str.append(QStringLiteral("%1\n").arg(resultStr));
        }
        else
        {
            str.append(QStringLiteral("# %1\nDetails:[%3 (%2)]\n").arg(scalarText).arg(diceList).arg(cmd));
        }
    }
    str.append(QStringLiteral("```"));
    out << str;
}
QString displaySVG(QString json, bool withColor)
{
    QJsonDocument doc= QJsonDocument::fromJson(json.toUtf8());
    auto obj= doc.object();
    auto error= obj["error"].toString();
    auto warning= obj["warning"].toString();
    auto comment= obj["warning"].toString();
    auto arrayInst= obj["instructions"].toArray();
    QStringList diceResults;
    for(auto inst : arrayInst)
    {
        auto obj= inst.toObject();
        auto diceVals= obj["diceval"].toArray();
        for(auto diceval : diceVals)
        {
            auto objval= diceval.toObject();
            auto resultStr= objval["string"].toString();
            diceResults << resultStr;
        }
    }
    auto diceList= diceResults.join(",");
    auto resultStr= obj["string"].toString();
    auto scalarText= obj["scalar"].toString();
    auto cmd= obj["command"].toString();

    QString str("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<svg version=\"1.1\"  "
                "xmlns=\"http://www.w3.org/2000/svg\" "
                "xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n");
    if(!error.isEmpty())
    {
        str.append(QStringLiteral("<text font-size=\"16\" x=\"0\" y=\"20\"><tspan "
                                  "fill=\"red\">%1</tspan></text>")
                       .arg(error));
    }
    else
    {
        if(!warning.isEmpty())
            str.append(QStringLiteral("<text font-size=\"16\" x=\"0\" y=\"20\"><tspan "
                                      "fill=\"orange\">%1</tspan></text>")
                           .arg(warning));

        int y= 20;
        if(!comment.isEmpty())
        {
            str.append(QStringLiteral("<text font-size=\"16\" x=\"0\" y=\"%2\"><tspan "
                                      "fill=\"blue\">%1</tspan></text>")
                           .arg(comment)
                           .arg(y));
            y+= 20;
        }
        // auto diceList= DisplayToolBox::diceToSvg(array, withColor, allSameColor, allSameFaceCount);
        if(!resultStr.isEmpty() && resultStr != scalarText)
        {
            resultStr.replace("%2", diceList.trimmed());
            str.append(QStringLiteral("<text font-size=\"16\" x=\"0\" y=\"%2\">%1</text>").arg(resultStr).arg(y));
        }
        else
        {
            if(withColor)
                str.append(QStringLiteral("<text font-size=\"16\" x=\"0\" y=\"%4\"><tspan "
                                          "fill=\"red\">%1</tspan>\n"
                                          "<tspan x=\"0\" y=\"%5\">details:</tspan>[%3 (%2)]</text>")
                               .arg(scalarText)
                               .arg(diceList)
                               .arg(cmd)
                               .arg(y)
                               .arg(y * 2));
            else
                str.append(QStringLiteral("<text font-size=\"16\" x=\"0\" y=\"%4\"><tspan>%1</tspan>\n"
                                          "<tspan x=\"0\" y=\"%5\">details:</tspan>[%3 (%2)]</text>")
                               .arg(scalarText)
                               .arg(diceList)
                               .arg(cmd)
                               .arg(y)
                               .arg(y * 2));
        }
    }
    str.append(QStringLiteral("</svg>\n"));
    return str;
}

#ifdef PAINTER_OP
void displayImage(QString json, bool withColor)
{
    auto svg= displaySVG(json, withColor);
    out << DisplayToolBox::makeImage(svg.toUtf8());
}
#endif

void displayCommandResult(QString json, bool withColor)
{
    QJsonDocument doc= QJsonDocument::fromJson(json.toUtf8());
    auto obj= doc.object();
    auto error= obj["error"].toString();
    auto warning= obj["warning"].toString();
    auto comment= obj["comment"].toString();
    auto arrayInst= obj["instructions"].toArray();
    QStringList diceResults;
    for(const auto& inst : qAsConst(arrayInst))
    {
        auto obj= inst.toObject();
        auto diceVals= obj["diceval"].toArray();
        for(const auto& diceval : qAsConst(diceVals))
        {
            auto objval= diceval.toObject();
            auto resultStr= objval["string"].toString();
            diceResults << resultStr;
        }
    }
    auto diceList= diceResults.join(",");
    auto scalarText= obj["scalar"].toString();
    auto cmd= obj["command"].toString();
    auto resultStr= obj["string"].toString();

    if(!error.isEmpty())
    {
        err << "Error" << error << "\n";
        return;
    }

    if(!warning.isEmpty())
        err << "Warning: " << warning << "\n";

    QString str;

    if(withColor)
        str= QString("Result: \e[0;31m%1\e[0m - details:[%3 (%2)]").arg(scalarText, diceList, cmd);
    else
        str= QString("Result: %1 - details:[%3 (%2)]").arg(scalarText, diceList, cmd);

    if(!resultStr.isEmpty() && resultStr != scalarText)
    {
        resultStr.replace("%2", diceList.trimmed());
        str= resultStr;
    }

    if(!comment.isEmpty())
    {
        if(withColor)
            out << "\033[1m" << comment << "\033[0m\n";
        else
            out << comment << " ";
    }
    out << str << "\n";
}

int startDiceParsing(QStringList& cmds, bool withColor, EXPORTFORMAT format, QJsonArray array, const QString& filePath)
{
    DiceParser parser;
    parser.insertAlias(new DiceAlias("L5R5R", QStringLiteral("L[-,⨀,⨀⬢,❂⬢,❁,❁⬢]")), 0);
    parser.insertAlias(new DiceAlias("L5R5S", QStringLiteral("L[-,-,⨀,⨀,⨀❁,⨀⬢,⨀⬢,❂,❂⬢,❁,❁,❁]")), 1);
    int i= 2;
    for(auto alias : array)
    {
        auto objAlias= alias.toObject();
        auto dice
            = new DiceAlias(objAlias["pattern"].toString(), objAlias["cmd"].toString(), !objAlias["regexp"].toBool());
        dice->setComment(objAlias["comment"].toString());
        parser.insertAlias(dice, i++);
    }

    int rt= 0;
    bool in_markdown= true;

    for(QString cmd : cmds)
    {
        if(cmd.startsWith('&') && format == BOT)
        {
            cmd= cmd.remove(0, 1);
            in_markdown= false;
        }

        if(parser.parseLine(cmd))
        {
            parser.start();

            std::set<EXPORTFORMAT> svgFormat({SVG, IMAGE, BOT});
            QString json;
            bool allSameColor= true;
            if(svgFormat.find(format) != svgFormat.end())
            {
                allSameColor= true;
                QString colorP;
                json= parser.resultAsJSon(
                    [&colorP, &allSameColor](const QString& value, const QString& color, bool) {
                        if(colorP.isNull())
                            colorP= color;
                        else if(colorP != color)
                            allSameColor= false;

                        return value;
                    },
                    true);

                if(!allSameColor)
                {
                    json= parser.resultAsJSon([](const QString& value, const QString& color, bool highlight) {
                        QString result= value;
                        bool hasColor= !color.isEmpty();
                        QString style;
                        if(hasColor)
                        {
                            style+= QStringLiteral("fill=\"%1\" ").arg(color);
                        }
                        if(highlight)
                        {
                            if(style.isEmpty())
                                style+= QStringLiteral("fill=\"%1\" ")
                                            .arg("red"); // default color must get the value from the setting object
                            style+= QStringLiteral("font-weight=\"bold\" ");
                        }
                        if(!style.isEmpty())
                            result= QString("<tspan %2>%1</tspan>").arg(value).arg(style);
                        return result;
                    });
                }
            }
            else if(TERMINAL == format)
            {
                allSameColor= true;
                QString colorP;
                json= parser.resultAsJSon(
                    [&colorP, &allSameColor](const QString& result, const QString& color, bool hightlight) {
                        auto trueColor= color;
                        if(color.isEmpty())
                            trueColor= "red";

                        if(colorP.isEmpty())
                            colorP= trueColor;
                        else if(colorP != trueColor)
                            allSameColor= false;

                        auto front= DisplayToolBox::colorToTermCode(trueColor);
                        auto end= front.isEmpty() ? "" : DisplayToolBox::colorToTermCode("reset");
                        return hightlight ? QString("%1%2%3").arg(front).arg(result).arg(end) : result;
                    });
            }
            else
            {
                allSameColor= true;
                json= parser.resultAsJSon([](const QString& result, const QString&, bool) { return result; });
            }

            if(format == BOT)
            {
                if(allSameColor)
                {
                    format= in_markdown ? MARKDOWN : TEXT;
                }
                else
                {
#ifdef PAINTER_OP
                    format= IMAGE;
#else
                    format= MARKDOWN;
#endif
                }
                if(!parser.humanReadableError().isEmpty())
                {
                    format= MARKDOWN;
                }
            }
            if(!withColor && format == TERMINAL)
                format= TEXT;

            // qDebug().noquote() << json << format;

            switch(format)
            {
            case TERMINAL:
                displayCommandResult(json, withColor);
                break;
            case SVG:
                out << displaySVG(json, withColor) << "\n";
                break;
            case BOT:
            case MARKDOWN:
                displayMarkdown(json);
                break;
            case TEXT:
                displayCommandResult(json, false);
                break;
            case JSON:
                displayJSon(json);
                break;
#ifdef PAINTER_OP
            case IMAGE:
                displayImage(json, withColor);
                break;
#endif
            }
            if(!filePath.isEmpty())
            {
                parser.writeDownDotTree(filePath);
            }
        }
        else
            rt= 1;
    }
    return rt;
}
#include <QTextCodec>
int main(int argc, char* argv[])
{
#ifdef PAINTER_OP
    QGuiApplication a(argc, argv);
#else
    QCoreApplication a(argc, argv);
#endif

    QStringList commands;
    QString cmd;
    QString dotFileStr;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    bool colorb= true;
    out.setCodec("UTF-8");
    EXPORTFORMAT format= TERMINAL;

    QCommandLineParser optionParser;
    QCommandLineOption color(QStringList() << "c"
                                           << "color-off",
                             "Disable color to highlight result");
    QCommandLineOption version(QStringList() << "v"
                                             << "version",
                               "Show the version and quit.");
    QCommandLineOption reset(QStringList() << "reset-settings", "Erase the settings and use the default parameters");
    QCommandLineOption alias(QStringList() << "a"
                                           << "alias",
                             "path to alias json files: <aliasfile>", "aliasfile");

    QCommandLineOption aliasData(QStringList() << "alias-data", "alias in json data <aliasdata>", "aliasdata");

    QCommandLineOption character(QStringList() << "s"
                                               << "charactersheet",
                                 "set Parameters to simulate character sheet: <sheetfile>", "sheetfile");
    QCommandLineOption markdown(QStringList() << "m"
                                              << "markdown",
                                "The output is formatted in markdown.");
    QCommandLineOption bot(QStringList() << "b"
                                         << "bot",
                           "Discord bot.");
    QCommandLineOption svg(QStringList() << "g"
                                         << "svg",
                           "The output is formatted in svg.");
    QCommandLineOption json(QStringList() << "j"
                                          << "json",
                            "The output is formatted in json.");
    QCommandLineOption line(QStringList() << "l"
                                          << "line",
                            "The output is in one line [default].");
    QCommandLineOption dotFile(QStringList() << "d"
                                             << "dot-file",
                               "Instead of rolling dice, generate the execution tree and write it in "
                               "<dotfile>",
                               "dotfile");
    QCommandLineOption translation(QStringList() << "t"
                                                 << "translation",
                                   "path to the translation file: <translationfile>", "translationfile");
    QCommandLineOption help(QStringList() << "h"
                                          << "help",
                            "Display this help");

    optionParser.addOption(color);
    optionParser.addOption(version);
    optionParser.addOption(reset);
    optionParser.addOption(dotFile);
    optionParser.addOption(alias);
    optionParser.addOption(aliasData);
    optionParser.addOption(character);
    optionParser.addOption(line);
    optionParser.addOption(markdown);
    optionParser.addOption(bot);
    optionParser.addOption(svg);
    optionParser.addOption(json);
    optionParser.addOption(translation);
    optionParser.addOption(help);
    for(int i= 0; i < argc; ++i)
    {
        commands << QString::fromUtf8(argv[i]);
    }

    optionParser.process(commands);

    if(optionParser.isSet(color))
    {
        commands.removeAt(0);
        colorb= false;
    }
    else if(optionParser.isSet(version))
    {
        out << "Rolisteam DiceParser v1.0.0"
            << "\n";
        out << "More Details: www.rolisteam.org"
            << "\n";
        return 0;
    }
    else if(optionParser.isSet(reset))
    {
        return 0;
    }
    else if(optionParser.isSet(dotFile))
    {
        dotFileStr= optionParser.value(dotFile);
    }
    if(optionParser.isSet(markdown))
    {
        format= MARKDOWN;
    }
    else if(optionParser.isSet(bot))
    {
        format= BOT;
    }
    else if(optionParser.isSet(svg))
    {
        format= SVG;
    }
    else if(optionParser.isSet(json))
    {
        format= JSON;
    }
    else if(optionParser.isSet(line))
    {
        format= TEXT;
    }

    if(optionParser.isSet(help))
    {
        cmd= "help";
    }
    QStringList cmdList= optionParser.positionalArguments();

    QJsonArray aliases;
    if(optionParser.isSet(alias))
    {
        auto aliasstr= optionParser.value(alias);

        QFile file(aliasstr);

        if(file.open(QIODevice::ReadOnly))
        {
            QJsonDocument doc= QJsonDocument::fromJson(file.readAll());
            aliases= doc.array();
        }
    }
    else if(optionParser.isSet(aliasData))
    {
        auto aliasstr= optionParser.value(aliasData);
        QJsonDocument doc= QJsonDocument::fromJson(aliasstr.toUtf8());
        aliases= doc.array();
    }

    returnValue= startDiceParsing(cmdList, colorb, format, aliases, dotFileStr);
    if(optionParser.isSet(help))
    {
        out << optionParser.helpText();
    }
    return returnValue;
}
