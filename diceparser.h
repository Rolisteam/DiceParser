#ifndef DICEPARSER_H
#define DICEPARSER_H

#include <QString>
#include <QMap>
#include "node/executionnode.h"

class Dice;

class DiceParser
{

public:
    enum DiceOperator {D};
    enum MathOperator {K,k,r,e};
    DiceParser();
    void parseLine(QString str);

private:
    bool readNumber(QString&  str, int& myNumber);
    bool readDice(QString&  str,Dice&);
    bool readDiceOperator(QString&,DiceOperator&);
    bool readDiceExpression(QString&,ExecutionNode* & node);
    bool readOperator(QString&);

    void setCurrentNode(ExecutionNode* node);

private:
    QMap<QString,DiceOperator>* m_mapDiceOp;
    ExecutionNode* m_start;
    ExecutionNode* m_current;
};

class Dice
{
public:
   DiceParser::DiceOperator m_diceOp;
   int m_faces;
};

#endif // DICEPARSER_H
