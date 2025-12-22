#include "displaytoolbox.h"
#include <QBuffer>
#include <QJsonArray>
#include <QJsonObject>
#include <sstream>

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

rang::fg DisplayToolBox::colorToIntCode(QString str)
{
    QHash<QString, rang::fg> data{
        {"black", rang::fg::black},   {"red", rang::fg::red},   {"green", rang::fg::green},
        {"yellow", rang::fg::yellow}, {"blue", rang::fg::blue}, {"magenta", rang::fg::magenta},
        {"cyan", rang::fg::cyan},     {"gray", rang::fg::gray}, {"reset", rang::fg::reset}};

    if(data.contains(str))
        return data.value(str);

    return rang::fg::black;
}

QString DisplayToolBox::colorToTermCode(QString str)
{
    rang::setControlMode(rang::control::Force);
    std::stringstream s;
    s << DisplayToolBox::colorToIntCode(str);
    return QString::fromStdString(s.str());
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
            for(auto valRef : std::as_const(values))
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

            for(auto valRef : std::as_const(values))
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
#include <QVariantList>

QString DisplayToolBox::diceResultToString(QJsonObject val, Output type, bool hasColor)
{
    auto total= QString::number(val["total"].toDouble());
    auto color= val["color"].toString();
    auto subvalues= val["subvalues"].toArray();
    QStringList subStr;

    for(auto subval : std::as_const(subvalues))
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
            total= QStringLiteral("%1%2%3").arg(DisplayToolBox::colorToTermCode(color), total,
                                                DisplayToolBox::colorToTermCode(QStringLiteral("reset")));
        }
        else if(type == Output::Svg)
        {
            total= QStringLiteral("%1%2%3").arg(QStringLiteral("<tspan fill=\"%1\">").arg(color), total,
                                                QStringLiteral("</tspan>"));
        }
    }
    return total;
}
