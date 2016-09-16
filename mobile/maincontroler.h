#ifndef MAINCONTROLER_H
#define MAINCONTROLER_H

#include <QObject>

class MainControler : public QObject
{
    Q_OBJECT
public:
    explicit MainControler(QObject *parent = 0);

signals:

public slots:
};

#endif // MAINCONTROLER_H
