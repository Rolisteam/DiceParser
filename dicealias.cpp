#include "dicealias.h"
#include <QRegularExpression>

DiceAlias::DiceAlias(QString cmd, QString key, bool isReplace)
    : m_command(cmd),m_value(key)
{
    if(isReplace)
    {
        m_type = REPLACE;
    }
    else
    {
        m_type = REGEXP;
    }
}

DiceAlias::~DiceAlias()
{

}

bool DiceAlias::resolved(QString & str)
{
    if((m_type == REPLACE)&&(str.contains(m_command)))
    {
       str.replace(m_command,m_value);
       return true;
    }
    else if(m_type == REGEXP)
    {
        QRegularExpression  exp(m_command);
        str.replace(exp,m_value);
        return true;
    }
    return false;
}

void DiceAlias::setCommand(QString key)
{
    m_command = key;
}

void DiceAlias::setValue(QString value)
{
    m_value = value;
}

void DiceAlias::setType(RESOLUTION_TYPE type)
{
    m_type = type;
}
QString DiceAlias::getCommand()
{
    return m_command;
}

QString DiceAlias::getValue()
{
    return m_value;
}

bool DiceAlias::isReplace()
{
    return (m_type == REPLACE) ? true : false;
}


