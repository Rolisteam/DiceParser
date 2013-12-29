#ifndef DICEPARSER_H
#define DICEPARSER_H

#include <QString>
#include <QMap>
#include "node/executionnode.h"

class Dice;
/**
 * @brief The DiceParser class facade class, it receives a command and return a DiceResult class (not yet implemented).
 */
class DiceParser
{

public:
    /**
     * @brief The DiceOperator enum gathering all dice operator
     */
    enum DiceOperator {D};
    /**
     * @brief The MathOperator enum gathering all dice option/math operator such as keep k, reroll r, explode e.
     */
    enum MathOperator {K,k,r,e};

    /**
     * @brief DiceParser default constructor
     */
    DiceParser();

    /**
     * @brief parseLine, method to call for starting the dice roll. It will parse the command and run the execution tree.
     * @param str dice command
     */
    void parseLine(QString str);

private:
    /**
     * @brief readNumber read number in the given str and remove from the string the read character.
     * @param str the command line
     * @param myNumber reference to the found number
     * @return true, succeed to read number, false otherwise.
     */
    bool readNumber(QString&  str, int& myNumber);
    /**
     * @brief readDice
     * @param str
     * @return
     */
    bool readDice(QString&  str,Dice&);
    /**
     * @brief readDiceOperator
     * @return
     */
    bool readDiceOperator(QString&,DiceOperator&);
    /**
     * @brief readDiceExpression
     * @param node
     * @return
     */
    bool readDiceExpression(QString&,ExecutionNode* & node);
    /**
     * @brief readOperator
     * @return
     */
    bool readOperator(QString&);
    /**
     * @brief setCurrentNode
     * @param node
     */
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
