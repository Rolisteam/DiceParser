#ifndef GENERATEIMAGE_H
#define GENERATEIMAGE_H

#include <QString>
#include <QJsonArray>

#include "diceparser.h"

class DisplayToolBox
{
public:
    DisplayToolBox();
    #ifdef PAINTER_OP
    static QString makeImage(QString scalarText, QString resultStr,QJsonArray array, bool withColor, QString cmd, QString comment, bool allSameFaceCount,bool allSameColor);
    #endif
    static QString colorToTermCode(QString str);
    static QString diceToText(QJsonArray array, bool withColor,bool allSameFaceCount, bool allSameColor);
    static QJsonArray diceToJson(QList<ExportedDiceResult> &diceList, bool &allSameFaceCount, bool &allSameColor);
    static QString diceToSvg(QJsonArray array, bool withColor, bool allSameColor, bool allSameFaceCount);
    static QString diceResultToString(QJsonObject val);
};


#endif // GENERATEIMAGE_H
