#ifndef DIE_H
#define DIE_H

#include <QList>

class Die
{
public:
    Die();

    void setValue(qint64 r);
    void setFaces(quint64 face);
    void insertRollValue(qint64 r);
    void setSelected(bool b);

    bool isSelected() const;
    qint64 getValue() const;
    QList<qint64> getListValue() const;

    bool hasChildrenValue();


    void roll(bool adding = false);
    void replaceLastValue(qint64 value);




private:
    qint64 m_value;
    QList<qint64> m_rollResult;
    bool m_selected;
    bool m_hasValue;
    quint64 m_faces;
};


#endif // DIE_H
