#ifndef GENERATEIMAGE_H
#define GENERATEIMAGE_H

#include <QImage>
#include <QString>
#include <QJsonArray>

#include "diceparser.h"

class DisplayToolBox
{
public:
    DisplayToolBox();
    static QString makeImage(QString scalarText, QString resultStr,QJsonArray array, bool withColor, QString cmd, QString comment, bool allSameFaceCount,bool allSameColor);
    static QString colorToTermCode(QString str);
    static QString diceToText(QJsonArray array, bool withColor,bool allSameFaceCount, bool allSameColor);
    static QJsonArray diceToJson(QList<ExportedDiceResult> &diceList, bool &allSameFaceCount, bool &allSameColor);
    static QString diceToSvg(QJsonArray array, bool withColor, bool allSameColor, bool allSameFaceCount);
    static QString diceResultToString(QJsonObject val);
};

struct Parsed {
    int isValid;
    QList<ExportedDiceResult> diceResult;
    QString result;
    QString comment;
    QString error;
    QString scalarText;
};

inline
Parsed parse(DiceParser& parser, QString cmd)
{
    Parsed parsed;

    parsed.isValid = parser.parseLine(cmd);
    if (!parsed.isValid) {
        return parsed;
    }

    parser.start();

    bool homogeneous = true;
    parser.getLastDiceResult(parsed.diceResult,homogeneous);

    QString lastScalarText;
    QStringList strLst;

    if(parser.hasIntegerResultNotInFirst())
    {
        auto values = parser.getLastIntegerResults();
        for(auto val : values )
        {
            strLst << QString::number(val);
        }
        parsed.scalarText = QString("%1").arg(strLst.join(','));
        lastScalarText = strLst.last();
    }
    else if(!parsed.diceResult.isEmpty())
    {
        auto values = parser.getSumOfDiceResult();
        for(auto val : values )
        {
            strLst << QString::number(val);
        }
        parsed.scalarText = QString("%1").arg(strLst.join(','));
    }

    if(parser.hasStringResult())
    {
        bool ok; // FIXME: use me
        parsed.result = parser.getAllStringResult(ok)
            .join(" ; ")
            .replace("%1",parsed.scalarText)
            .replace("%3",lastScalarText);

        int i = 1;
        for(auto value : strLst)
        {
            parsed.result.replace(QStringLiteral("$%1").arg(i),value);
            ++i;
        }
    }

    parsed.comment = parser.getComment();
    parsed.error = parser.humanReadableError();

    return parsed;
}


#endif // GENERATEIMAGE_H
