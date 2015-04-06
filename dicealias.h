#ifndef DICEALIAS_H
#define DICEALIAS_H

#include <QString>

class DiceAlias
{
public:
    enum RESOLUTION_TYPE { REPLACE,REGEXP};
    DiceAlias(QString cmd, QString key, bool isReplace = true);
    ~DiceAlias();

    bool resolved(QString & str);

    void setCommand(QString key);
    void setValue(QString value);
    void setType(RESOLUTION_TYPE );

    QString getCommand();
    QString getValue();
    bool isReplace();
private:
    QString m_command;
    QString m_value;
    RESOLUTION_TYPE m_type;

};

#endif // DICEALIAS_H
