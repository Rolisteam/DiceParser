#ifndef STRINGRESULT_H
#define STRINGRESULT_H

#include <QString>
#include "result.h"
/**
 * @brief The StringResult class stores command result for String.
 */
class StringResult : public Result
{
public:
    /**
     * @brief StringResult
     */
    StringResult();
	/**
	 * @brief StringResult
	 */
	virtual ~StringResult();
    /**
     * @brief setText
     * @param text
     */
    void setText(QString text);
    /**
     * @brief getText
     * @return
     */
    QString getText() const;
    /**
     * @brief getScalar
     * @return
     */
    virtual QVariant getResult(RESULT_TYPE);
    /**
     * @brief toString
     * @return
     */
    virtual QString toString(bool);

    virtual void setHighLight(bool );
    virtual bool hasHighLight() const;
    virtual bool hasResultOfType(RESULT_TYPE resultType) const;
private:
    QString m_value;
    bool m_highlight;
};

#endif // STRINGRESULT_H
