#ifndef STRINGRESULT_H
#define STRINGRESULT_H

#include <QString>
#include "result.h"
/**
 * @brief The StringResult class
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

private:
    QString m_value;
};

#endif // STRINGRESULT_H
