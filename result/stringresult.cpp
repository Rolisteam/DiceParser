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
QString StringResult::toString(bool wl)
{
	if(wl)
	{
		return QString("%2 [label=\"StringResult_value_%1\"]").arg(getText().replace(" ","_")).arg(m_id);
	}
	else
	{
		return m_id;
	}
}
void StringResult::setHighLight(bool b)
{
    m_highlight = b;
}

bool StringResult::hasHighLight() const
{
    return m_highlight;
}
