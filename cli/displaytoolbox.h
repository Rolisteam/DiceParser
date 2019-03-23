#ifndef GENERATEIMAGE_H
#define GENERATEIMAGE_H

#include <QJsonArray>
#include <QString>

#include "diceparser.h"

class DisplayToolBox
{
public:
    enum class Output
    {
        Svg,
        Terminal,
        Json,
        Image
    };
    DisplayToolBox();
#ifdef PAINTER_OP
    static QString makeImage(QByteArray svgCode);
#endif
    static QString colorToTermCode(QString str);
    static QString diceToText(QJsonArray array, bool withColor, bool allSameFaceCount, bool allSameColor);
    static QJsonArray diceToJson(QList<ExportedDiceResult>& diceList, bool& allSameFaceCount, bool& allSameColor);
    static QString diceToSvg(QJsonArray array, bool withColor, bool allSameColor, bool allSameFaceCount);
    static QString diceResultToString(QJsonObject val, Output type, bool hasColor);
};

#endif // GENERATEIMAGE_H
