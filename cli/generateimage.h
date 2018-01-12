#ifndef GENERATEIMAGE_H
#define GENERATEIMAGE_H

#include <QImage>
#include <QString>
#include <QJsonArray>

class ImageGenerator
{
public:
    ImageGenerator();
    static QString makeImage(QJsonArray array,bool allSameFaceCount);
};


#endif // GENERATEIMAGE_H
