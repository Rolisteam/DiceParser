#ifndef STRINGRESULT_H
#define STRINGRESULT_H

#include "diceresult.h"
#include <QString>
/**
 * @brief The StringResult class stores command result for String.
 */

class StringResult : public DiceResult
{
public:
    /**
     * @brief StringResult
     */
    StringResult();
    /**
     * @brief StringResult
     */
    virtual ~StringResult() override;
    void addText(QString text);
    void finished();
    QString getText() const;
    virtual QVariant getResult(Dice::RESULT_TYPE) override;
    virtual QString toString(bool) override;

    virtual void setHighLight(bool);
    virtual bool hasHighLight() const;
    virtual bool hasResultOfType(Dice::RESULT_TYPE resultType) const override;
    virtual Result* getCopy() const override;

    bool isDigitOnly() const;

    void setStringCount(int count);
    QString getStringResult() const override;
    void setNoComma(bool b);

private:
    QStringList m_value;
    bool m_highlight= true;
    int m_stringCount= 0;
    bool m_commaSeparator= true;
};

#endif // STRINGRESULT_H
