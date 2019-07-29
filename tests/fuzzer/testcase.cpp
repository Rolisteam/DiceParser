#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <diceparser.h>

static DiceParser* parser= new DiceParser();

void runCommand(const QString& cmd)
{
    qDebug() << "cmd" << cmd;
    if(parser->parseLine(cmd))
    {
        parser->start();
    }
}

int main(int argc, char** argv)
{
    // qDebug() << "first";
    QCoreApplication app(argc, argv);

    // qDebug() << "start";
    QFile file(app.arguments().at(1));
    // qDebug() << "file" << app.arguments().at(1);
    if(!file.open(QIODevice::ReadOnly))
        return 1;

    auto line= file.readLine();
    while(!line.isEmpty())
    {
        // qDebug() << line;
        runCommand(QString::fromUtf8(line));
        line= file.readLine();
    }

    return 0;
}
