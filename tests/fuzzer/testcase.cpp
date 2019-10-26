#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QtConcurrent>
#include <diceparser.h>
#include <vector>

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
    QCoreApplication app(argc, argv);
    QDir dir("/home/renaud/application/mine/DiceParser/tests/fuzzer/sync_dir/fuzzer06/crashes");
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Name);

    QFileInfoList list= dir.entryInfoList();
    for(auto fileInfo : list)
    {
        // QFile file(app.arguments().at(1));
        QFile file(fileInfo.absoluteFilePath());

        if(!file.open(QIODevice::ReadOnly))
            return 1;

        auto line= file.readLine();
        while(!line.isEmpty())
        {
            // qDebug() << line;
            runCommand(QString::fromUtf8(line));
            line= file.readLine();
        }
    }
    return 0;
}
