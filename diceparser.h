#ifndef DICEPARSER_H
#define DICEPARSER_H

#include <QString>
#include <QMap>

#include "node/executionnode.h"
#include "node/dicerollernode.h"

#include "validator.h"
#include "range.h"
#include "booleancondition.h"
#include "parsingtoolbox.h"

/**
 * @mainpage DiceParser
 *
 * the grammar is something like this:
 *
 * Command =: Expression | ScalarOperator Expression
 * Expression =: number | number Dice | Command
 * Dice =: DiceOperator Number(faces)
 * DiceOperator =: D
 * ScalarOperator =: [x,-,*,x,/]
 * number =: [0-9]+
 *
 */

class Dice;
/**
 * @brief The DiceParser class facade class, it receives a command and return a DiceResult class (not yet implemented).
 */
class DiceParser
{

public:
    /**
     * @brief The DiceOperator enum gathering all dice operators
     */
    enum DiceOperator {D};



    /**
     * @brief The OptionOperator enum gathering all options  availables for result.
     */
    enum OptionOperator {KeepAndExplose,keep,Reroll,Explosing,Sort,Count};

    /**
     * @brief DiceParser default constructor
     */
    DiceParser();

    /**
     * @brief parseLine, method to call for starting the dice roll. It will parse the command and run the execution tree.
     * @param str dice command
     * @return bool every thing is fine or not
     */
    bool parseLine(QString str);


    /**
     * @brief Start running the execution tree
     *
     */
    void Start();

    /**
     * @brief displayResult
     */
    void displayResult();

    bool readExpression(QString& str,ExecutionNode* & node);

private:

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
    bool readDiceExpression(QString&,ExecutionNode*  & node);
    /**
     * @brief readOperator
     * @return
     */
    bool readOperator(QString&,ExecutionNode* previous);
    /**
     * @brief setCurrentNode
     * @param node
     */
    ExecutionNode* getLatestNode(ExecutionNode* node);

    /**
     * @brief readOption
     */
    bool readOption(QString&,ExecutionNode* node, bool hasDice = true);

    DiceRollerNode* addRollDiceNode(qint64 faces,ExecutionNode*);

private:
    QMap<QString,DiceOperator>* m_mapDiceOp;
    QMap<QString,OptionOperator>* m_OptionOp;
    QMap<QString,QString>* m_aliasMap;

    ExecutionNode* m_start;
    ExecutionNode* m_current;
    QString m_command;
    ParsingToolBox* m_parsingToolbox;
};

/**
 * @brief The Dice class is a basic class to store dice. (May not be required any more).
 */
class Dice
{
public:
   DiceParser::DiceOperator m_diceOp;
   int m_faces;
};

#endif // DICEPARSER_H
