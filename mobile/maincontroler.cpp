#include "maincontroler.h"

#include <QQmlContext>
#include <QDebug>

MainControler::MainControler(QObject *parent)
    : QObject(parent)
{
    m_model = new CommandModel();
    m_model->insertCmd("L5R","8D10e10k4");

    m_diceParser = new DiceParser();
}
void MainControler::initEngine(QQmlApplicationEngine* engine)
{
    m_engine = engine;
    engine->rootContext()->setContextProperty("_model",m_model);
    connect(m_engine,SIGNAL(objectCreated(QObject*,QUrl)),this,SLOT(setConnections(QObject*,QUrl)));


}
void MainControler::rollDice(QString cmd)
{
    if(m_diceParser->parseLine(cmd))
    {
        m_diceParser->start();
        for(int i = 0; i < m_diceParser->getStartNodeCount(); ++i)
        {
           //qDebug() << m_diceParser->getSumOfDiceResult(i);
        }
    }
}
void MainControler::setConnections(QObject* root,QUrl url)
{
    //QObject* root = engine->rootContext()->contextObject();
    connect(root,SIGNAL(roll(QString)),this,SLOT(rollDice(QString)));
    connect(root,SIGNAL(addRoll(QString,QString)),m_model,SLOT(insertCmd(QString,QString)));
}
