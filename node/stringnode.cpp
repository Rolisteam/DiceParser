#include "stringnode.h"

StringNode::StringNode()
    : m_stringResult(new StringResult())
{
    m_result = m_stringResult;
}

void StringNode::run(ExecutionNode *previous)
{
    m_previousNode = previous;
    if(NULL!=previous)
    {
        m_result->setPrevious(previous->getResult());
    }
    if(NULL!=m_nextNode)
    {
        m_nextNode->run(this);
    }
}

void StringNode::setString(QString str)
{
    m_data = str;
    m_stringResult->setText(m_data);
}
QString StringNode::toString(bool withLabel) const
{
    if(withLabel)
    {
        return QString("%1 [label=\"StringNode %2\"]").arg(m_id).arg(m_data);
    }
    else
    {
        return m_id;
    }
}
/*void StringNode::getScalarResult()
{
    QString scalarText;

    if(m_diceParser->hasIntegerResultNotInFirst())
    {
        scalarText = QStringLiteral("%1").arg(m_diceParser->getLastIntegerResult());
    }
    else if(hasDiceList)
    {
        scalarText = QStringLiteral("%1").arg(m_diceParser->getSumOfDiceResult());
    }
}*/

/*bool StringNode::getMessageResult(QString& value, QString& command, QString& list)
{
    QString scalarText;
    QString diceText;
    //QString pattern("");



    bool hasDiceList = false;
    if(m_diceParser->hasDiceResult())
    {
        ExportedDiceResult diceList;
        bool ok;
        m_diceParser->getLastDiceResult(diceList,ok);//fills the ExportedDiceResult
        diceText = diceToText(diceList);
        hasDiceList= true;
    }
    if(m_diceParser->hasSeparator())
    {
        bool ok;
        QStringList allStringlist = m_diceParser->getAllDiceResult(ok);
        if(ok)
        {
            QString patternColor("<span class=\"dice\">%1</span>");
            list =   patternColor.arg(allStringlist.join(' '));
            scalarText = list;
        }
    }
    else if(m_diceParser->hasIntegerResultNotInFirst())
    {
        scalarText = QStringLiteral("%1").arg(m_diceParser->getLastIntegerResult());
    }
    else if(hasDiceList)
    {
        scalarText = QStringLiteral("%1").arg(m_diceParser->getSumOfDiceResult());
    }
    value=scalarText;
    list = diceText.trimmed();
    command = m_diceParser->getDiceCommand().toHtmlEscaped();
    if(m_diceParser->hasStringResult())
    {
        bool ok;
        QStringList allStringlist = m_diceParser->getAllStringResult(ok);
        if(ok)
        {
            QString patternColor("<span class=\"dice\">%1</span>");
            list =   patternColor.arg(allStringlist.join(' '));
            value = list;
        }
        else
        {
            value = m_diceParser->getStringResult().replace("\n","<br/>");
            list = allStringlist.join(' ');
            return true;
        }
    }

    return false;
}*/
qint64 StringNode::getPriority() const
{
    qint64 priority=0;
    if(NULL!=m_nextNode)
    {
        priority = m_nextNode->getPriority();
    }


    return priority;
}
