#include "generateimage.h"

ImageGenerator::ImageGenerator()
{

}

QString ImageGenerator::makeImage(QJsonArray array,bool allSameFaceCount)
{
    QString lengthStr("%1");
    int length=0;

    if(allSameFaceCount)
    {
        QStringList result;
        for(auto item : array)
        {
            auto obj = item.toObject();
            auto values= obj["values"].toArray();
            for(auto val : values)
            {
                result.append(val.toString());
            }
        }
        return result.join(',');
    }
    else
    {
        QStringList result;
        for(auto item : array)
        {
            QStringList subResult;
            auto obj = item.toObject();
            auto values= obj["values"].toArray();
            for(auto val : values)
            {
                result.append(val.toString());
            }
            result.append(QStringLiteral("d%1:(").arg(obj["face"].toString()));
            result.append(subResult.join(','));
            result.append(QStringLiteral(")"));

        }
        return result.join(' ');
    }
    QImage img;
    img.fill(Qt::transparent);
}
