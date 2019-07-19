#include "displaytoolbox.h"
#include <QBuffer>
#include <QJsonArray>
#include <QJsonObject>

#ifdef PAINTER_OP
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QSvgRenderer>
#endif

#include <QDebug>

#define LINE_SPACING 5

DisplayToolBox::DisplayToolBox() {}
#ifdef PAINTER_OP
QString DisplayToolBox::makeImage(QByteArray svgCode)
{
    QSvgRenderer svg(svgCode);

    QImage image(500, 60, QImage::Format_ARGB32);
    image.fill(QColor(255, 255, 255, 100)); // partly transparent red-ish background

    // Get QPainter that paints to the image
    QPainter painter(&image);
    svg.render(&painter);
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return ba.toBase64();
}
#endif

QString DisplayToolBox::colorToTermCode(QString str)
{
    if(str.isEmpty() || str == QStringLiteral("black"))
    {
        return QStringLiteral("\e[0;31m");
    }
    else if(str == QStringLiteral("white"))
    {
        return QStringLiteral("\e[97m");
    }
    else if(str == QStringLiteral("blue"))
    {
        return QStringLiteral("\e[34m");
    }
    else if(str == QStringLiteral("red"))
    {
        return QStringLiteral("\e[31m");
    }
    else if(str == QStringLiteral("black"))
    {
        return QStringLiteral("\e[30m");
    }
    else if(str == QStringLiteral("green"))
    {
        return QStringLiteral("\e[32m");
    }
    else if(str == QStringLiteral("yellow"))
    {
        return QStringLiteral("\e[33m");
    }
    else if(str == QStringLiteral("reset"))
    {
        return QStringLiteral("\e[0m");
    }
    return {};
}

QString DisplayToolBox::diceToSvg(QJsonArray array, bool withColor, bool allSameColor, bool allSameFaceCount)
{
    Q_UNUSED(allSameColor)
    if(allSameFaceCount)
    {
        QStringList result;
        for(auto item : array)
        {
            QStringList subResult;
            auto obj= item.toObject();
            auto values= obj["values"].toArray();
            for(auto valRef : values)
            {
                subResult.append(diceResultToString(valRef.toObject(), Output::Svg, withColor));
            }
            result.append(subResult.join(','));
        }
        return result.join("");
    }
    else
    {
        QStringList result;
        for(auto item : array)
        {
            QStringList subResult;
            auto obj= item.toObject();
            auto values= obj["values"].toArray();

            for(auto valRef : values)
            {
                subResult.append(diceResultToString(valRef.toObject(), Output::Svg, withColor));
            }
            result.append(QStringLiteral("d%1:(").arg(obj["face"].toInt()));
            if(withColor)
            {
                result.append(QStringLiteral("<tspan fill=\"%1\">").arg(obj["color"].toString()));
            }
            result.append(subResult.join(','));
            if(withColor)
            {
                result.append(QStringLiteral("</tspan>)"));
            }
            else
            {
                result.append(QStringLiteral(")"));
            }
        }
        return result.join("");
    }
}
QJsonArray DisplayToolBox::diceToJson(QList<ExportedDiceResult>& diceList, bool& allSameFaceCount, bool& allSameColor)
{
    allSameFaceCount= true;

    QJsonArray array;
    QStringList colorList;
    for(auto dice : diceList)
    {
        if(dice.size() > 1)
        {
            allSameFaceCount= false;
        }
        for(quint64 face : dice.keys())
        {
            ListDiceResult diceResults= dice.value(face);
            QJsonObject object;
            QJsonArray values;
            object["face"]= static_cast<int>(face);
            for(auto const& dice : diceResults)
            {
                QJsonObject diceObj;
                auto listValues= dice.getResult();
                if(!listValues.isEmpty())
                {
                    diceObj["total"]= static_cast<qint64>(listValues.takeFirst());
                    diceObj["face"]= static_cast<int>(face);
                    auto color= dice.getColor();
                    diceObj["color"]= color;
                    if(!colorList.contains(color))
                        colorList.append(color);
                    QJsonArray subValues;
                    for(auto result : listValues)
                    {
                        subValues.push_back(static_cast<qint64>(result));
                    }
                    diceObj["subvalues"]= subValues;
                }
                values.push_back(diceObj);
            }
            object["values"]= values;
            array.push_back(object);
        }
    }
    if(colorList.size() > 1)
        allSameColor= false;
    return array;
}
QString DisplayToolBox::diceResultToString(QJsonObject val, Output type, bool hasColor)
{
    auto total= QString::number(val["total"].toDouble());
    auto color= val["color"].toString();
    auto subvalues= val["subvalues"].toArray();
    QStringList subStr;

    for(auto subval : subvalues)
    {
        subStr << QString::number(subval.toDouble());
    }
    if(!subStr.isEmpty())
    {
        total.append(QStringLiteral(" [%1]").arg(subStr.join(',')));
    }
    if(hasColor && !color.isEmpty())
    {
        if(type == Output::Terminal)
        {
            total= QStringLiteral("%1%2%3")
                       .arg(DisplayToolBox::colorToTermCode(color))
                       .arg(total)
                       .arg(DisplayToolBox::colorToTermCode(QStringLiteral("reset")));
        }
        else if(type == Output::Svg)
        {
            total= QStringLiteral("%1%2%3")
                       .arg(QStringLiteral("<tspan fill=\"%1\">").arg(color))
                       .arg(total)
                       .arg(QStringLiteral("</tspan>"));
        }
    }
    return total;
}
QString DisplayToolBox::diceToText(QJsonArray array, bool withColor, bool allSameFaceCount, bool allSameColor)
{
    Q_UNUSED(allSameColor)
    QStringList result;
    for(auto item : array)
    {
        QString subResult("");
        auto obj= item.toObject();
        auto values= obj["values"].toArray();

        QStringList diceResult;
        for(auto valRef : values)
        {
            diceResult+= diceResultToString(valRef.toObject(), Output::Terminal, withColor);
        }
        if(!diceResult.isEmpty())
        {
            if(!allSameFaceCount)
            {
                subResult+= QStringLiteral("d%1:(").arg(obj["face"].toInt());
            }
            if(withColor)
            {
                subResult+= DisplayToolBox::colorToTermCode(obj["color"].toString());
            }
            subResult+= diceResult.join(" ");
            if(withColor)
            {
                subResult+= DisplayToolBox::colorToTermCode(QStringLiteral("reset"));
            }
            if(!allSameFaceCount)
            {
                subResult+= QStringLiteral(")");
            }
        }

        if(!subResult.isEmpty())
        {
            result+= subResult;
        }
    }
    return result.join(" - ");
}
