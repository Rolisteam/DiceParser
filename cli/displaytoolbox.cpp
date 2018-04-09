#include "displaytoolbox.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QFontMetrics>
#include <QPainter>
#include <QBuffer>
#include <QFont>
#include <QDebug>

#define LINE_SPACING 5

DisplayToolBox::DisplayToolBox()
{

}

QString DisplayToolBox::makeImage(QString scalarText, QString resultStr,QJsonArray array, bool withColor, QString cmd, QString comment, bool allSameFaceCount,bool allSameColor)
{
    QString textTotal("%1 Details:[%3 %2]");


    int lineCount=2;
    if(!comment.isEmpty())
    {
        lineCount = 3;
    }

    if(resultStr.isEmpty())
    {
        auto list = diceToText(array,false,allSameFaceCount,allSameColor);
        textTotal = textTotal.arg(scalarText).arg(list).arg(cmd);
    }
    else
    {
        --lineCount;
        textTotal = resultStr;
    }
    if(comment.size()>textTotal.size())
    {
        textTotal.prepend(QStringLiteral("%1\n").arg(comment));
    }
    QFont font("Helvetica", 15);// = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    QFontMetrics fm(font);
    QRect rect = fm.boundingRect(textTotal);

    QImage img(rect.width(),(rect.height()+LINE_SPACING)*lineCount,QImage::Format_ARGB32);
    img.fill(Qt::transparent);
    QPainter painter(&img);
    painter.setFont(font);
    int y = rect.height();
    if(!comment.isNull())
    {
        QPen pen = painter.pen();
        pen.setColor(Qt::black);
        painter.setPen(pen);
        painter.drawText(QPoint(0,y),comment);
        y += rect.height()+LINE_SPACING;
    }
    if(!resultStr.isEmpty())
    {
        QPen pen = painter.pen();
        pen.setColor(Qt::black);
        painter.setPen(pen);
        painter.drawText(QPoint(0,y),resultStr);
    }
    else
    {
        painter.save();
        QPen pen = painter.pen();
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawText(QPoint(0,y),scalarText);
        y += rect.height()+LINE_SPACING;
        painter.restore();
        int x = 0;
        QString text=QStringLiteral("Details:[%1 (").arg(cmd);
        painter.drawText(QPoint(x,y),text);
        x += fm.boundingRect(text).width();

        if(allSameFaceCount)
        {
            int i = 0;
            for(auto item : array)
            {
                QStringList result;
                bool last = (array.size()-1==i);
                auto obj = item.toObject();
                auto values= obj["values"].toArray();
                for(auto valRef : values)
                {
                    result.append(diceResultToString(valRef.toObject()));
                }
                painter.save();
                QPen pen = painter.pen();
                QColor color;
                color.setNamedColor(obj["color"].toString());
                pen.setColor(color);

                painter.setPen(pen);
                text = QStringLiteral("%1").arg(result.join(','));
                if(!last)
                {
                    text.append(",");
                }
                painter.drawText(QPoint(x,y),text);
                x += fm.boundingRect(text).width();
                painter.restore();
                ++i;
            }
        }
        else
        {
            int i = 0;
            for(auto item : array)
            {
                QStringList result;
                auto obj = item.toObject();
                bool last = (array.size()-1==i);
                auto values= obj["values"].toArray();
                for(auto valRef : values)
                {
                    result.append(diceResultToString(valRef.toObject()));
                }
                text =QStringLiteral("d%1:(").arg(obj["face"].toDouble());
                painter.drawText(QPoint(x,y),text);
                x += fm.boundingRect(text).width();

                painter.save();
                QPen pen = painter.pen();
                QColor color;
                color.setNamedColor(obj["color"].toString());
                pen.setColor(color);
                painter.setPen(pen);

                text = QStringLiteral("%1").arg(result.join(','));
                painter.drawText(QPoint(x,y),text);
                x += fm.boundingRect(text).width();
                painter.restore();

                text = QStringLiteral(")");
                if(!last)
                {
                    text.append(",");
                }
                painter.drawText(QPoint(x,y),text);
                x += fm.boundingRect(text).width();
                ++i;
            }
        }
        text = QStringLiteral(")");
        x += fm.boundingRect(text).width();
        painter.drawText(QPoint(x,y),text);
    }

    painter.end();

    img.save("/home/renaud/image.png","PNG");
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG");

    //return {};
    return ba.toBase64();
}

QString DisplayToolBox::colorToTermCode(QString str)
{
    if(str.isEmpty()|| str==QStringLiteral("black"))
    {
        return QStringLiteral("\e[0;31m");
    }
    else if(str==QStringLiteral("white"))
    {
        return  QStringLiteral("\e[97m");
    }
    else if(str==QStringLiteral("blue"))
    {
        return  QStringLiteral("\e[34m");
    }
    else if(str==QStringLiteral("red"))
    {
        return  QStringLiteral("\e[31m");
    }
    else if(str==QStringLiteral("black"))
    {
        return  QStringLiteral("\e[30m");
    }
    else if(str==QStringLiteral("green"))
    {
        return  QStringLiteral("\e[32m");
    }
    else if(str==QStringLiteral("yellow"))
    {
        return  QStringLiteral("\e[33m");
    }
    else if(str==QStringLiteral("reset"))
    {
        return  QStringLiteral("\e[0m");
    }
    return {};
}

QString DisplayToolBox::diceToSvg(QJsonArray array,bool withColor,bool allSameColor,bool allSameFaceCount )
{
    Q_UNUSED(allSameColor)
    if(allSameFaceCount)
    {
        QStringList result;
        for(auto item : array)
        {
            QStringList subResult;
            auto obj = item.toObject();
            auto values= obj["values"].toArray();
            for(auto valRef : values)
            {
                subResult.append(diceResultToString(valRef.toObject()));
            }
            if(withColor)
            {
                result.append(QStringLiteral("<tspan fill=\"%1\">").arg(obj["color"].toString()));
                result.append(subResult.join(','));
                result.append(QStringLiteral("</tspan>"));
            }
            else
            {
                result.append(subResult.join(','));
            }
        }
        return result.join("");
    }
    else
    {
        QStringList result;
        for(auto item : array)
        {
            QStringList subResult;
            auto obj = item.toObject();
            auto values= obj["values"].toArray();

            for(auto valRef : values)
            {
                subResult.append(diceResultToString(valRef.toObject()));
            }
            result.append(QStringLiteral("d%1:(").arg(obj["face"].toString()));
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
QJsonArray DisplayToolBox::diceToJson(QList<ExportedDiceResult>& diceList,bool& allSameFaceCount,bool& allSameColor)
{
    allSameFaceCount = true;
    allSameColor = true;
    QJsonArray array;
    for(auto dice : diceList)
    {
        if(dice.size()>1)
        {
            allSameFaceCount = false;
        }
        for(int face:  dice.keys())
        {
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
                sameColorDice[i].push_back(dice);
            }
            int i = 0;
            if(alreadyDoneColor.size()>1)
            {
                allSameColor = false;
            }
            for(auto it = alreadyDoneColor.begin() ; it != alreadyDoneColor.end(); ++it)
            {
                auto list = sameColorDice[i];
                QJsonObject object;
                object["color"]=*it;
                object["face"]=face;
                QJsonArray values;
                for(auto const dice : list)
                {
                    QJsonObject diceObj;
                    auto listValues = dice.getResult();
                    if(!listValues.isEmpty())
                    {
                        diceObj["total"]=(qint64)listValues.takeFirst();
                        QJsonArray subValues;
                        for(auto result : listValues)
                        {
                            subValues.push_back((qint64)result);
                        }
                        diceObj["subvalues"]=subValues;
                    }
                    values.push_back(diceObj);
                }
                object["values"]=values;
                ++i;
                array.push_back(object);
            }
        }
    }
    return array;
}
QString DisplayToolBox::diceResultToString(QJsonObject val)
{
    auto total = QString::number(val["total"].toDouble());
    auto subvalues = val["subvalues"].toArray();
    QStringList subStr;
    for(auto subval : subvalues)
    {
         subStr << QString::number(subval.toDouble());
    }
    if(!subStr.isEmpty())
    {
        total.append(QStringLiteral(" [%1]").arg(subStr.join(',')));
    }
    return total;
}
QString DisplayToolBox::diceToText(QJsonArray array, bool withColor,bool allSameFaceCount, bool allSameColor)
{
    Q_UNUSED(allSameColor)
	QStringList result;
	for(auto item : array)
	{
		QString subResult;
		auto obj = item.toObject();
		auto values= obj["values"].toArray();

		QStringList diceResult;
		for(auto valRef : values)
		{
			diceResult += diceResultToString(valRef.toObject());
		}
		if (!diceResult.isEmpty())
		{
			if(!allSameFaceCount)
			{
				subResult += QStringLiteral("d%1:(").arg(obj["face"].toString());
			}
			if(withColor)
			{
				subResult += DisplayToolBox::colorToTermCode(obj["color"].toString());
			}
			subResult += diceResult.join(" ");
			if(withColor)
			{
				subResult += DisplayToolBox::colorToTermCode(QStringLiteral("reset"));
			}
			if(!allSameFaceCount)
			{
				subResult += QStringLiteral(")");
			}
		}

		if (!subResult.isEmpty())
		{
			result += subResult;
		}
	}
	return result.join(" - ");
}
