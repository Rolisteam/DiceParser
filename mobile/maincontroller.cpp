#include "maincontroler.h"

#include <QJSValue>
#include <QQmlEngine>

MainControler::MainControler(QObject* parent) : QObject(parent)
{

    m_diceParser= new DiceParser();
    qmlRegisterSingletonType("DiceParser", 1, 0, "Model", [](QQmlEngine* engine, QJSEngine* scriptEngine) -> QObject* {
        Q_UNUSED(engine)
        static CommandModel model;
        static bool initialized= false;
        if(!initialized)
        {
            model.insertCmd("L5R", "8D10e10k4");
            initialized= true;
        }
        return &model;
    });
}
void MainControler::initEngine(QQmlApplicationEngine* engine)
{
    m_engine= engine;
    engine->rootContext()->setContextProperty("_model", m_model);
    connect(m_engine, SIGNAL(objectCreated(QObject*, QUrl)), this, SLOT(setConnections(QObject*, QUrl)));
}
void MainControler::rollDice(QString cmd)
{
    if(m_diceParser->parseLine(cmd))
    {
        m_diceParser->start();
        for(int i= 0; i < m_diceParser->getStartNodeCount(); ++i)
        {
            // qDebug() << m_diceParser->getSumOfDiceResult(i);
        }
    }
}
void MainControler::setConnections(QObject* root, QUrl url)
{
    // QObject* root = engine->rootContext()->contextObject();
    connect(root, SIGNAL(roll(QString)), this, SLOT(rollDice(QString)));
    connect(root, SIGNAL(addRoll(QString, QString)), m_model, SLOT(insertCmd(QString, QString)));
}
