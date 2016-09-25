#ifndef MAINCONTROLER_H
#define MAINCONTROLER_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "commandmodel.h"
#include "diceparser.h"

class MainControler : public QObject
{
    Q_OBJECT
public:
    explicit MainControler(QObject *parent = 0);

    void initEngine(QQmlApplicationEngine* );
signals:

public slots:
    void setConnections(QObject* root,QUrl url);
    void rollDice(QString cmd);
private:
    CommandModel* m_model;
    DiceParser* m_diceParser;
    QQmlApplicationEngine* m_engine;
};

#endif // MAINCONTROLER_H
