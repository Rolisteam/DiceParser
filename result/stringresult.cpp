#include "stringresult.h"
#include <QDebug>

StringResult::StringResult()
{
    m_highlight= true;
    m_resultTypes= static_cast<int>(Dice::RESULT_TYPE::STRING);
}
void StringResult::addText(QString text)
{
    m_value.append(text);
}
StringResult::~StringResult() {}
bool StringResult::hasResultOfType(Dice::RESULT_TYPE resultType) const
{
    bool val= false;

    switch(resultType)
    {
    case Dice::RESULT_TYPE::STRING:
        val= !isDigitOnly();
        break;
    case Dice::RESULT_TYPE::SCALAR:
        val= isDigitOnly();
        break;
    case Dice::RESULT_TYPE::DICE_LIST:
        val= (isDigitOnly() && m_value.size() > 1);
        break;
    default:
        break;
    }
    return val;
}
QString StringResult::getText() const
{
    return m_value.join(",");
}
QVariant StringResult::getResult(Dice::RESULT_TYPE type)
{
    switch(type)
    {
    case Dice::RESULT_TYPE::STRING:
        return getText();
    case Dice::RESULT_TYPE::SCALAR:
        return getScalarResult();
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

void StringResult::finished()
{
    if(isDigitOnly())
    {
        std::for_each(m_value.begin(), m_value.end(), [this](const QString& str) {
            auto die= new Die();
            die->setMaxValue(m_stringCount);
            die->setValue(str.toInt());
            insertResult(die);
        });
    }
}

void StringResult::setStringCount(int count)
{
    m_stringCount= count;
}

bool StringResult::isDigitOnly() const
{
    return std::all_of(m_value.begin(), m_value.end(), [](const QString& str) {
        bool ok= false;
        str.toInt(&ok);
        return ok;
    });
}

Result* StringResult::getCopy() const
{
    auto copy= new StringResult();
    copy->setPrevious(getPrevious());
    copy->setHighLight(m_highlight);
    std::for_each(m_value.begin(), m_value.end(), [copy](const QString& str) { copy->addText(str); });
    return copy;
}
