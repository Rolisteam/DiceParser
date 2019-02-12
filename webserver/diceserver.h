#include "diceparser.h"
#include "qhttp/src/qhttpserver.hpp"
#include <QObject>

class DiceServer : public QObject
{
    Q_OBJECT
public:
    DiceServer(int port= 8085);
    virtual ~DiceServer();

    QString startDiceParsing(QString cmd);
    QString diceToText(ExportedDiceResult& dice, bool highlight, bool homogeneous);

private:
    DiceParser* m_diceParser;
    qhttp::server::QHttpServer* m_server;
};
