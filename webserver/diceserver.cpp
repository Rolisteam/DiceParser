#include "diceserver.h"
#include "qhttp/src/qhttpserver.hpp"
#include "qhttp/src/qhttpserverrequest.hpp"
#include "qhttp/src/qhttpserverresponse.hpp"
#include "qhttp/src/qhttpfwd.hpp"
#include <QHostAddress>
#include <QUrl>

DiceServer::DiceServer(int port)
    : QObject(),m_diceParser(new DiceParser())
{
   // using namespace ;
    m_server = new qhttp::server::QHttpServer(this);
    m_server->listen( // listening on 0.0.0.0:8080
            QHostAddress::Any, port,
            [=](qhttp::server::QHttpRequest* req, qhttp::server::QHttpResponse* res)
            {
                req->collectData(1024);

               // qhttp::THeaderHash hash = req->headers();
               // qDebug() << hash << res->headers() << qhttp::Stringify::toString(req->method()) << qPrintable(req->url().toString()) << req->collectedData().constData();
                QString getArg = req->url().toString();
                getArg=getArg.replace("/?","");
                QStringList args = getArg.split('&');
                QHash<QString,QString> m_hashArgs ;
                for( auto argument : args)
                {
                    QStringList keyValue = argument.split('=');
                    if(keyValue.size()==2)
                    {
                        m_hashArgs.insert(keyValue[0],keyValue[1]);
                    }
                }

                if(m_hashArgs.contains("cmd"))
                {
                    qDebug() << QUrl::fromPercentEncoding(m_hashArgs["cmd"].toLocal8Bit());
                    QString result = startDiceParsing(QUrl::fromPercentEncoding(m_hashArgs["cmd"].toLocal8Bit()));

                    res->setStatusCode(qhttp::ESTATUS_OK);
                    QString html("<!doctype html>\n"
                             "<html>\n"
                             "<head>\n"
                             "</head>\n"
                             "  <meta charset=\"utf-8\">\n"
                             "  <title>Rolisteam Dice System Webservice</title>\n"
                             "  <style>.dice {color:#FF0000;font-weight: bold;}</style>"
                             "<body>\n"
                             "%1\n"
                             "</body>\n"
                             "</html>\n");

                    res->end(html.arg(result).toLocal8Bit());
                }
                else
                {
                    res->setStatusCode(qhttp::ESTATUS_OK);
                    res->end("No Command found!\n");
                }

        });
    if ( !m_server->isListening() ) {
            qDebug() << "failed to listen";

        }
    else
    {
        qDebug()<< "Server is On!!";
    }
}

DiceServer::~DiceServer()
{
    qDebug()<< "destructor";
}
QString DiceServer::diceToText(ExportedDiceResult& dice,bool highlight,bool homogeneous)
{
    QStringList resultGlobal;
    foreach(int face, dice.keys())
    {
        QStringList result;
        QStringList currentStreak;
        QList<QStringList> allStreakList;
        ListDiceResult diceResult =  dice.value(face);
        bool previousHighlight=false;
        QString previousColor;
        QString patternColor("<span class=\"dice\">");
        foreach (HighLightDice tmp, diceResult)
        {
            if(previousColor != tmp.getColor())
            {
                if(!currentStreak.isEmpty())
                {
                    QStringList list;
                    list << patternColor+currentStreak.join(',')+"</span>";
                    allStreakList.append(list);
                    currentStreak.clear();
                }
                if(tmp.getColor().isEmpty())
                {
                    patternColor = QStringLiteral("<span class=\"dice\">");
                }
                else
                {
                    patternColor = QStringLiteral("<span style=\"color:%1;font-weight:bold\">").arg(tmp.getColor());
                }
            }
            QStringList diceListStr;
            if((previousHighlight)&&(!tmp.isHighlighted()))
            {
                if(!currentStreak.isEmpty())
                {
                    QStringList list;
                    list << patternColor+currentStreak.join(',')+"</span>";
                    allStreakList.append(list);
                    currentStreak.clear();
                }

            }
            else if((!previousHighlight)&&(tmp.isHighlighted()))
            {
                if(!currentStreak.isEmpty())
                {
                    QStringList list;
                    list << currentStreak.join(',');
                    allStreakList.append(list);
                    currentStreak.clear();
                }
            }
            previousHighlight = tmp.isHighlighted();
            previousColor = tmp.getColor();
            for(int i =0; i < tmp.getResult().size(); ++i)
            {
                qint64 dievalue = tmp.getResult()[i];
                diceListStr << QString::number(dievalue);
            }
            if(diceListStr.size()>1)
            {
                QString first = diceListStr.takeFirst();
                first = QString("%1 [%2]").arg(first).arg(diceListStr.join(','));
                diceListStr.clear();
                diceListStr << first;
            }
            currentStreak << diceListStr.join(' ');
        }

        if(previousHighlight)
        {
            QStringList list;
            list <<  patternColor+currentStreak.join(',')+"</span>";
            allStreakList.append(list);
        }
        else
        {
            if(!currentStreak.isEmpty())
            {
                QStringList list;
                list << currentStreak.join(',');
                allStreakList.append(list);
            }
        }
        foreach(QStringList a, allStreakList)
        {
            result << a;
        }
        if(dice.keys().size()>1)
        {
            resultGlobal << QString(" d%2:(%1)").arg(result.join(",")).arg(face);
        }
        else
        {
            resultGlobal << result.join(",");
        }
    }
    return resultGlobal.join("");
}

QString DiceServer::startDiceParsing(QString cmd)
{
    QString result("");
    bool highlight = true;
    if(m_diceParser->parseLine(cmd))
    {
            m_diceParser->Start();
            if(!m_diceParser->getErrorMap().isEmpty())
            {
                result +=  "<span style=\"color: #FF0000\">Error:</span>" + m_diceParser->humanReadableError() + "<br/>";
            }
            else
            {

                ExportedDiceResult list;
                bool homogeneous = true;
                m_diceParser->getLastDiceResult(list,homogeneous);
                QString diceText = diceToText(list,highlight,homogeneous);
                QString scalarText;
                QString str;

                if(m_diceParser->hasIntegerResultNotInFirst())
                {
                    scalarText = QString("%1").arg(m_diceParser->getLastIntegerResult());
                }
                else if(!list.isEmpty())
                {
                    scalarText = QString("%1").arg(m_diceParser->getSumOfDiceResult());
                }
                if(highlight)
                {
                    str = QString("Result: <span class=\"dice\">%1</span>, details:[%3 (%2)]").arg(scalarText).arg(diceText).arg(m_diceParser->getDiceCommand());
                }
                else
                {
                    str = QString("Result: %1, details:[%3 (%2)]").arg(scalarText).arg(diceText).arg(m_diceParser->getDiceCommand());
                }

                if(m_diceParser->hasStringResult())
                {
                    str = m_diceParser->getStringResult();
                }
                result += str + "<br/>";
            }
    }
    else
    {
        result += "<span style=\"color: #00FF00\">Error:</span>" + m_diceParser->humanReadableError() + "<br/>";
    }


    return result;
}
