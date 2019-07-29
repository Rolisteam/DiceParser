#include "stringresult.h"

StringResult::StringResult()
{
    m_highlight= true;
    m_resultTypes= static_cast<int>(Dice::RESULT_TYPE::STRING);
}
void StringResult::setText(QString text)
{
    m_value= text;
}
StringResult::~StringResult() {}
bool StringResult::hasResultOfType(Dice::RESULT_TYPE resultType) const
{

    if(resultType == Dice::RESULT_TYPE::STRING)
    {
        return true;
    }
    else if(resultType == Dice::RESULT_TYPE::SCALAR)
    {
        bool ok= false;
        getText().toInt(&ok);
        return ok;
    }
    return false;
}
QString StringResult::getText() const
{
    return m_value;
}
QVariant StringResult::getResult(Dice::RESULT_TYPE type)
{
    switch(type)
    {
    case Dice::RESULT_TYPE::STRING:
        return getText();
    case Dice::RESULT_TYPE::SCALAR:
        return getText().toInt();
    default:
        return QVariant();
    }
}
QString StringResult::toString(bool wl)
{
    if(wl)
    {
        return QString("%2 [label=\"StringResult_value_%1\"]").arg(getText().replace("%", "_"), m_id);
    }
    else
    {
        return m_id;
    }
}
void StringResult::setHighLight(bool b)
{
    m_highlight= b;
}

bool StringResult::hasHighLight() const
{
    return m_highlight;
}
Result* StringResult::getCopy() const
{
    auto copy= new StringResult();
    copy->setHighLight(m_highlight);
    copy->setText(m_value);
    return copy;
}
