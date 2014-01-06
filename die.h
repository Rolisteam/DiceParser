#ifndef DIE_H
#define DIE_H

#include <QList>

class Die
{
public:
    Die();

    void setValue(qint64 r);
    void insertRollValue(qint64 r);
    void setSelected(bool b);

    bool isSelected() const;
    qint64 getValue() const;
    QList<qint64> getListValue() const;

private:
    qint64 m_value;
    QList<qint64> m_rollResult;
    bool m_selected;
};


#endif // DIE_H
