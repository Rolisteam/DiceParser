/***************************************************************************
 *	Copyright (C) 2017 by Renaud Guezennec                             *
 *   https://rolisteam.org/contact                   *
 *                                                                         *
 *   Rolisteam is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <diceparser_qobject/diceroller.h>

#include <QJsonObject>
#include <QtConcurrent>

DiceRoller::DiceRoller(QObject* parent) : QObject(parent) {}

DiceRoller::~DiceRoller()= default;

QString DiceRoller::diceList() const
{
    return m_diceList;
}

QString DiceRoller::resultStr() const
{
    return m_resultStr;
}

QString DiceRoller::command() const
{
    return m_command;
}

qreal DiceRoller::result() const
{
    return m_result;
}

void DiceRoller::setCommand(const QString& cmd)
{
    if(m_command != cmd)
    {
        m_command= cmd;
        emit commandChanged();
    }
}

void DiceRoller::readErrorAndWarning()
{
    setError(
        tr("Error:\n%1\nWarnings:\n%2").arg(m_diceparser.humanReadableError(), m_diceparser.humanReadableWarning()));
}

void DiceRoller::start()
{
    auto future= QtConcurrent::run([this]() {
        if(m_diceparser.parseLine(m_command))
        {
            m_diceparser.start();
            readErrorAndWarning();
            auto jsonstr= m_diceparser.resultAsJSon([](const QString& value, const QString&, bool) { return value; });
            QJsonDocument doc= QJsonDocument::fromJson(jsonstr.toLocal8Bit());
            auto json= doc.object();
            m_result= json["scalar"].toString().toDouble();
            emit resultChanged();
        }
    });
}

QString DiceRoller::error() const
{
    return m_error;
}

QList<DiceAlias*>* DiceRoller::aliases() const
{
    return m_diceparser.aliases();
}

DiceParser* DiceRoller::parser()
{
    return &m_diceparser;
}

void DiceRoller::setError(const QString& error)
{
    if(m_error == error)
        return;

    m_error= error;
    emit errorOccurs();
}

/*QString DiceRoller::diceToText(QList<ExportedDiceResult>& diceList)
{
    QStringList global;
    for(auto& dice : diceList)
    {
        QStringList resultGlobal;
        auto const& keys= dice.keys();
        for(auto& face : keys)
        {
            QStringList result;
            auto list= dice.value(face);
            for(auto diceResult : list)
            {
                for(const HighLightDice& tmp : diceResult)
                {
                    QStringList diceListStr;
                    QStringList diceListChildren;
                    int i= 0;
                    for(qint64& dievalue : tmp.result())
                    {
                        QString prefix("%1");
                        if(i == 0)
                        {
                            diceListStr << prefix.arg(QString::number(dievalue));
                        }
                        else
                        {
                            diceListChildren << prefix.arg(QString::number(dievalue));
                        }
                        ++i;
                    }
                    if(!diceListChildren.isEmpty())
                    {
                        diceListStr << QString("[%1]").arg(diceListChildren.join(' '));
                    }
                    result << diceListStr.join(' ');
                }

if(keys.size() > 1)
{
    resultGlobal << QString(" d%2:(%1)").arg(result.join(',')).arg(face);
}
else
{
    resultGlobal << result;
}
}
}
global << resultGlobal.join(' ');
}
return global.join(" ; ");
}*/

/*if(m_diceparser.parseLine(m_command))
    {
        m_diceparser.start();
        if(m_diceparser.errorMap().isEmpty())
        {
            bool homogeneous;
            QList<ExportedDiceResult> list;
            m_diceparser.lastDiceResult(list, homogeneous);
            QString diceText= diceToText(list);
            QString scalarText;
            QString str;

qreal result= 0;
QString resultStr;
if(m_diceparser.hasIntegerResultNotInFirst())
{
    auto values= m_diceparser.lastIntegerResults();
    QStringList strLst;
    for(auto& val : values)
    {
        result+= val;
        strLst << QString::number(val);
    }
    scalarText= QString("%1").arg(strLst.join(','));
}
else if(!list.isEmpty())
{
    auto values= m_diceparser.getSumOfDiceResult();
    QStringList strLst;
    for(auto val : values)
    {
        result+= val;
        strLst << QString::number(val);
    }
    scalarText= QString("%1").arg(strLst.join(','));
}

if(m_diceparser.hasStringResult())
{
    bool ok;
    QStringList allStringlist= m_diceparser.getAllStringResult(ok);
    QString stringResult= allStringlist.join(" ; ");
    stringResult.replace("%1", scalarText);
    stringResult.replace("%2", diceText.trimmed());
    str= stringResult;
}
else
{
    resultStr= scalarText;
}
if(!m_diceparser.getComment().isEmpty())
{
    resultStr+= m_diceparser.getComment() + "\n";
}
resultStr+= str + "\n";
m_resultStr= resultStr;
m_result= result;
m_diceList= diceText.trimmed();
emit resultStrChanged();
emit resultChanged();
emit diceListChanged();
}
}

if(!m_diceparser.getErrorMap().isEmpty())
{
    auto errors= m_diceparser.getErrorMap();
    setError(errors.first());
}
*/
