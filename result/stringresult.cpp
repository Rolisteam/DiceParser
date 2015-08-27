#include "stringresult.h"

StringResult::StringResult()
{
    m_highlight = true;
    m_resultTypes = Result::STRING;
}
void StringResult::setText(QString text)
{
    m_value=text;
}
StringResult::~StringResult()
{

}

QString StringResult::getText() const
{
    return m_value;
}
QVariant StringResult::getResult(RESULT_TYPE type)
{

    switch(type)
    {
    case STRING:
        return getText();
        break;

    }


    return QVariant();
}
QString StringResult::toString()
{
    return QString("StringResult_value_%1").arg(getText().replace(" ","_"));
}
void StringResult::setHighLight(bool b)
{
    m_highlight = b;
}

bool StringResult::hasHighLight() const
{
    return m_highlight;
}
