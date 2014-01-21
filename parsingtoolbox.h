#ifndef PARSINGTOOLBOX_H
#define PARSINGTOOLBOX_H

#include <QMap>

#include "node/executionnode.h"
#include "booleancondition.h"
#include "range.h"

class ParsingToolBox
{
public:
    ParsingToolBox();
    ExecutionNode* addSort(ExecutionNode* e,bool b);
    bool readLogicOperator(QString& str,BooleanCondition::LogicOperator& op);
    Validator* readValidator(QString& str);


    /**
     * @brief readNumber read number in the given str and remove from the string the read character.
     * @param str the command line
     * @param myNumber reference to the found number
     * @return true, succeed to read number, false otherwise.
     */
    bool readNumber(QString&  str, int& myNumber);


    bool readOpenParentheses(QString& str);
    bool readCloseParentheses(QString& str);

private:
        QMap<QString,BooleanCondition::LogicOperator>* m_logicOp;
};

#endif // PARSINGTOOLBOX_H
