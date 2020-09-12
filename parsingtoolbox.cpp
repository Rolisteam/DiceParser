/***************************************************************************
 * Copyright (C) 2014 by Renaud Guezennec                                   *
 * https://rolisteam.org/contact                      *
 *                                                                          *
 *  This file is part of DiceParser                                         *
 *                                                                          *
 * DiceParser is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation; either version 2 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with this program; if not, write to the                            *
 * Free Software Foundation, Inc.,                                          *
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.                 *
 ***************************************************************************/
#include "parsingtoolbox.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QRegularExpression>
#include <QString>
#include <set>

#include "node/allsamenode.h"
#include "node/bind.h"
#include "node/countexecutenode.h"
#include "node/dicerollernode.h"
#include "node/executionnode.h"
#include "node/explodedicenode.h"
#include "node/filternode.h"
#include "node/groupnode.h"
#include "node/helpnode.h"
#include "node/ifnode.h"
#include "node/jumpbackwardnode.h"
#include "node/keepdiceexecnode.h"
#include "node/listaliasnode.h"
#include "node/listsetrollnode.h"
#include "node/mergenode.h"
#include "node/numbernode.h"
#include "node/occurencecountnode.h"
#include "node/paintnode.h"
#include "node/parenthesesnode.h"
#include "node/repeaternode.h"
#include "node/rerolldicenode.h"
#include "node/scalaroperatornode.h"
#include "node/sortresult.h"
#include "node/splitnode.h"
#include "node/startingnode.h"
#include "node/stringnode.h"
#include "node/uniquenode.h"
#include "node/valueslistnode.h"
#include "node/variablenode.h"

QHash<QString, QString> ParsingToolBox::m_variableHash;

ParsingToolBox::ParsingToolBox()
{
    // m_logicOp = ;
    m_logicOp.insert(">=", BooleanCondition::GreaterOrEqual);
    m_logicOp.insert("<=", BooleanCondition::LesserOrEqual);
    m_logicOp.insert("<", BooleanCondition::LesserThan);
    m_logicOp.insert("=", BooleanCondition::Equal);
    m_logicOp.insert(">", BooleanCondition::GreaterThan);
    m_logicOp.insert("!=", BooleanCondition::Different);

    // m_logicOperation = ;
    m_logicOperation.insert("|", ValidatorList::OR);
    m_logicOperation.insert("^", ValidatorList::EXCLUSIVE_OR);
    m_logicOperation.insert("&", ValidatorList::AND);

    // m_conditionOperation = ;
    m_conditionOperation.insert("%", OperationCondition::Modulo);

    // m_arithmeticOperation = new QHash<QString,ScalarOperatorNode::ArithmeticOperator>();
    m_arithmeticOperation.push_back({QStringLiteral("**"), Die::POW});
    m_arithmeticOperation.push_back({QStringLiteral("+"), Die::PLUS});
    m_arithmeticOperation.push_back({QStringLiteral("-"), Die::MINUS});
    m_arithmeticOperation.push_back({QStringLiteral("*"), Die::MULTIPLICATION});
    m_arithmeticOperation.push_back({QStringLiteral("x"), Die::MULTIPLICATION});
    m_arithmeticOperation.push_back({QStringLiteral("|"), Die::INTEGER_DIVIDE});
    m_arithmeticOperation.push_back({QStringLiteral("/"), Die::DIVIDE});
    m_arithmeticOperation.push_back({QStringLiteral("÷"), Die::DIVIDE});

    m_mapDiceOp.insert(QStringLiteral("D"), D);
    m_mapDiceOp.insert(QStringLiteral("L"), L);

    m_OptionOp.insert(QStringLiteral("k"), Keep);
    m_OptionOp.insert(QStringLiteral("K"), KeepAndExplode);
    m_OptionOp.insert(QStringLiteral("s"), Sort);
    m_OptionOp.insert(QStringLiteral("c"), Count);
    m_OptionOp.insert(QStringLiteral("r"), Reroll);
    m_OptionOp.insert(QStringLiteral("e"), Explode);
    m_OptionOp.insert(QStringLiteral("R"), RerollUntil);
    m_OptionOp.insert(QStringLiteral("a"), RerollAndAdd);
    m_OptionOp.insert(QStringLiteral("m"), Merge);
    m_OptionOp.insert(QStringLiteral("i"), ifOperator);
    m_OptionOp.insert(QStringLiteral("p"), Painter);
    m_OptionOp.insert(QStringLiteral("f"), Filter);
    m_OptionOp.insert(QStringLiteral("y"), Split);
    m_OptionOp.insert(QStringLiteral("u"), Unique);
    m_OptionOp.insert(QStringLiteral("t"), AllSameExplode);
    m_OptionOp.insert(QStringLiteral("g"), Group);
    m_OptionOp.insert(QStringLiteral("b"), Bind);
    m_OptionOp.insert(QStringLiteral("o"), Occurences);

    m_functionMap.insert({QStringLiteral("repeat"), REPEAT});

    m_nodeActionMap.insert(QStringLiteral("@"), JumpBackward);

    m_commandList.append(QStringLiteral("help"));
    m_commandList.append(QStringLiteral("la"));
}

ParsingToolBox::ParsingToolBox(const ParsingToolBox&) {}
ParsingToolBox::~ParsingToolBox() {}

void ParsingToolBox::clearUp()
{
    m_errorMap.clear();
    m_comment= QString("");
}

void ParsingToolBox::cleanUpAliases()
{
    m_aliasList.clear();
}

ExecutionNode* ParsingToolBox::addSort(ExecutionNode* e, bool b)
{
    SortResultNode* nodeSort= new SortResultNode();
    nodeSort->setSortAscending(b);
    e->setNextNode(nodeSort);
    return nodeSort;
}
ExecutionNode* ParsingToolBox::getLeafNode(ExecutionNode* start)
{
    ExecutionNode* next= start;
    while(nullptr != next->getNextNode())
    {
        next= next->getNextNode();
    }
    return next;
}
void ParsingToolBox::addError(Dice::ERROR_CODE code, const QString& msg)
{
    m_errorMap.insert(code, msg);
}
void ParsingToolBox::addWarning(Dice::ERROR_CODE code, const QString& msg)
{
    m_warningMap.insert(code, msg);
}
bool ParsingToolBox::readDiceLogicOperator(QString& str, OperationCondition::ConditionOperator& op)
{
    QString longKey;
    auto const& keys= m_conditionOperation.keys();
    for(const QString& tmp : keys)
    {
        if(str.startsWith(tmp))
        {
            if(longKey.size() < tmp.size())
            {
                longKey= tmp;
            }
        }
    }
    if(longKey.size() > 0)
    {
        str= str.remove(0, longKey.size());
        op= m_conditionOperation.value(longKey);
        return true;
    }

    return false;
}

bool ParsingToolBox::readArithmeticOperator(QString& str, Die::ArithmeticOperator& op)
{

    auto it= std::find_if(
        m_arithmeticOperation.begin(), m_arithmeticOperation.end(),
        [str](const std::pair<QString, Die::ArithmeticOperator>& pair) { return str.startsWith(pair.first); });
    if(it == m_arithmeticOperation.end())
        return false;

    op= it->second;
    str= str.remove(0, it->first.size());
    return true;
}

bool ParsingToolBox::readLogicOperator(QString& str, BooleanCondition::LogicOperator& op)
{
    QString longKey;
    auto const& keys= m_logicOp.keys();
    for(const QString& tmp : keys)
    {
        if(str.startsWith(tmp))
        {
            if(longKey.size() < tmp.size())
            {
                longKey= tmp;
            }
        }
    }
    if(longKey.size() > 0)
    {
        str= str.remove(0, longKey.size());
        op= m_logicOp.value(longKey);
        return true;
    }

    return false;
}
QString ParsingToolBox::getComment() const
{
    return m_comment;
}

void ParsingToolBox::setComment(const QString& comment)
{
    m_comment= comment;
}
const QMap<Dice::ERROR_CODE, QString>& ParsingToolBox::getErrorList() const
{
    return m_errorMap;
}
const QMap<Dice::ERROR_CODE, QString>& ParsingToolBox::getWarningList() const
{
    return m_warningMap;
}
bool ParsingToolBox::readOperand(QString& str, ExecutionNode*& node)
{
    qint64 intValue= 1;
    QString resultStr;
    if(readDynamicVariable(str, intValue))
    {
        VariableNode* variableNode= new VariableNode();
        variableNode->setIndex(static_cast<quint64>(intValue - 1));
        variableNode->setData(&m_startNodes);
        node= variableNode;
        return true;
    }
    else if(readNumber(str, intValue))
    {
        NumberNode* numberNode= new NumberNode();
        numberNode->setNumber(intValue);
        node= numberNode;
        return true;
    }
    else if(readString(str, resultStr))
    {
        StringNode* strNode= new StringNode();
        strNode->setString(resultStr);
        node= strNode;
        return true;
    }
    return false;
}

Validator* ParsingToolBox::readValidator(QString& str, bool hasSquare)
{
    Validator* returnVal= nullptr;
    auto opCompare= readConditionType(str);
    BooleanCondition::LogicOperator myLogicOp= BooleanCondition::Equal;
    readLogicOperator(str, myLogicOp);

    OperationCondition::ConditionOperator condiOp= OperationCondition::Modulo;
    bool hasDiceLogicOperator= readDiceLogicOperator(str, condiOp);
    ExecutionNode* operandNode= nullptr;
    if(hasDiceLogicOperator)
    {
        if(readOperand(str, operandNode))
        {
            OperationCondition* condition= new OperationCondition();
            condition->setConditionType(opCompare);
            condition->setValueNode(operandNode);
            Validator* valid= readValidator(str, hasSquare);
            BooleanCondition* boolC= dynamic_cast<BooleanCondition*>(valid);
            if(nullptr != boolC)
            {
                condition->setBoolean(boolC);
                returnVal= condition;
            }
            else
            {
                delete condition;
            }
        }
    }
    else if(readOperand(str, operandNode))
    {
        bool isRange= false;
        if(str.startsWith("..") && hasSquare)
        {
            str= str.remove(0, 2);
            qint64 end= 0;
            if(readNumber(str, end))
            {
                str= str.remove(0, 1);
                qint64 start= operandNode->getScalarResult();
                Range* range= new Range();
                range->setConditionType(opCompare);
                range->setValue(start, end);
                returnVal= range;
                isRange= true;
            }
        }

        if(!isRange)
        {
            BooleanCondition* condition= new BooleanCondition();
            condition->setConditionType(opCompare);
            condition->setValueNode(operandNode);
            condition->setOperator(myLogicOp);
            returnVal= condition;
        }
    }
    return returnVal;
}

Dice::ConditionType ParsingToolBox::readConditionType(QString& str)
{
    Dice::ConditionType type= Dice::OnEach;
    if(str.startsWith('.'))
    {
        str= str.remove(0, 1);
        type= Dice::OneOfThem;
    }
    else if(str.startsWith('?'))
    {
        str= str.remove(0, 1);
        type= Dice::OnEachValue;
    }
    else if(str.startsWith('*'))
    {
        str= str.remove(0, 1);
        type= Dice::AllOfThem;
    }
    else if(str.startsWith(':'))
    {
        str= str.remove(0, 1);
        type= Dice::OnScalar;
    }
    return type;
}
bool ParsingToolBox::hasError() const
{
    return !m_errorMap.isEmpty();
}
ValidatorList* ParsingToolBox::readValidatorList(QString& str)
{
    bool expectSquareBrasket= false;
    if((str.startsWith("[")))
    {
        str= str.remove(0, 1);
        expectSquareBrasket= true;
    }
    Validator* tmp= readValidator(str, expectSquareBrasket);
    ValidatorList::LogicOperation opLogic;

    QVector<ValidatorList::LogicOperation> operators;
    QList<Validator*> validatorList;

    while(nullptr != tmp)
    {
        bool hasOperator= readLogicOperation(str, opLogic);
        if(hasOperator)
        {
            operators.append(opLogic);
            validatorList.append(tmp);
            tmp= readValidator(str, expectSquareBrasket);
        }
        else
        {
            if((expectSquareBrasket) && (str.startsWith("]")))
            {
                str= str.remove(0, 1);
                // isOk=true;
            }
            validatorList.append(tmp);
            tmp= nullptr;
        }
    }
    if(!validatorList.isEmpty())
    {
        ValidatorList* validator= new ValidatorList();
        validator->setOperationList(operators);
        validator->setValidators(validatorList);
        return validator;
    }
    else
    {
        return nullptr;
    }
}
bool ParsingToolBox::readLogicOperation(QString& str, ValidatorList::LogicOperation& op)
{
    QString longKey;
    auto const& keys= m_logicOperation.keys();
    for(auto& tmp : keys)
    {
        if(str.startsWith(tmp))
        {
            if(longKey.size() < tmp.size())
            {
                longKey= tmp;
            }
        }
    }
    if(longKey.size() > 0)
    {
        str= str.remove(0, longKey.size());
        op= m_logicOperation.value(longKey);
        return true;
    }

    return false;
}

bool ParsingToolBox::readNumber(QString& str, qint64& myNumber)
{
    if(str.isEmpty())
        return false;

    QString number;
    int i= 0;
    while(i < str.length() && ((str[i].isNumber()) || ((i == 0) && (str[i] == '-'))))
    {
        number+= str[i];
        ++i;
    }

    if(number.isEmpty())
    {
        QString reason;
        return readVariable(str, myNumber, reason);
    }

    bool ok;
    myNumber= number.toLongLong(&ok);
    if(ok)
    {
        str= str.remove(0, number.size());
        return true;
    }

    return false;
}
bool ParsingToolBox::readDynamicVariable(QString& str, qint64& index)
{
    if(str.isEmpty())
        return false;
    if(str.startsWith('$'))
    {
        QString number;
        int i= 1;
        while(i < str.length() && (str[i].isNumber()))
        {
            number+= str[i];
            ++i;
        }

        bool ok;
        index= number.toLongLong(&ok);
        if(ok)
        {
            str= str.remove(0, number.size() + 1);
            return true;
        }
    }
    return false;
}

ExecutionNode* ParsingToolBox::getLatestNode(ExecutionNode* node)
{
    if(nullptr == node)
        return nullptr;

    ExecutionNode* next= node;
    while(nullptr != next->getNextNode())
    {
        next= next->getNextNode();
    }
    return next;
}

const std::vector<ExecutionNode*>& ParsingToolBox::getStartNodes()
{
    return m_startNodes;
}

QStringList ParsingToolBox::allFirstResultAsString(bool& hasAlias) const
{
    // QStringList allResult;
    QStringList stringListResult;
    for(auto node : m_startNodes)
    {
        QVariant var;
        auto stringPair= hasResultOfType(Dice::RESULT_TYPE::STRING, node);
        auto scalarPair= hasResultOfType(Dice::RESULT_TYPE::SCALAR, node, true);
        if(stringPair.first)
        {
            stringListResult << stringPair.second.toString();
            hasAlias= true;
        }
        else if(scalarPair.first)
        {
            stringListResult << QString::number(scalarPair.second.toReal());
            hasAlias= true;
        }
    }
    return stringListResult;
}
std::pair<bool, QVariant> ParsingToolBox::hasResultOfType(Dice::RESULT_TYPE type, ExecutionNode* node,
                                                          bool notthelast) const
{
    bool hasValidResult= false;
    QVariant var;
    ExecutionNode* next= ParsingToolBox::getLeafNode(node);
    Result* result= next->getResult();
    while((result != nullptr) && (!hasValidResult))
    {
        bool lastResult= false;
        if(notthelast)
            lastResult= (nullptr == result->getPrevious());

        if(result->hasResultOfType(type) && !lastResult)
        {
            hasValidResult= true;
            var= result->getResult(type);
        }
        result= result->getPrevious();
    }
    return {hasValidResult, var};
}

QList<qreal> ParsingToolBox::scalarResultsFromEachInstruction() const
{
    QList<qreal> resultValues;
    std::set<QString> alreadyVisitedNode;
    for(auto node : m_startNodes)
    {
        ExecutionNode* next= ParsingToolBox::getLeafNode(node);
        Result* result= next->getResult();
        bool scalarDone= false;
        while((result != nullptr) && (!scalarDone))
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::SCALAR))
            {
                if(alreadyVisitedNode.find(result->getId()) == alreadyVisitedNode.end())
                {
                    resultValues << result->getResult(Dice::RESULT_TYPE::SCALAR).toReal();
                    alreadyVisitedNode.insert(result->getId());
                }
                scalarDone= true;
            }
            result= result->getPrevious();
        }
    }
    return resultValues;
}

QList<qreal> ParsingToolBox::sumOfDiceResult() const
{
    QList<qreal> resultValues;
    for(auto node : m_startNodes)
    {
        qreal resultValue= 0;
        ExecutionNode* next= ParsingToolBox::getLeafNode(node);
        Result* result= next->getResult();
        bool found= false;
        while((nullptr != result) && (!found))
        {
            if(result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
            {
                DiceResult* myDiceResult= dynamic_cast<DiceResult*>(result);
                if(nullptr != myDiceResult)
                {
                    for(auto& die : myDiceResult->getResultList())
                    {
                        resultValue+= die->getValue();
                    }
                    found= true;
                }
            }
            result= result->getPrevious();
        }
        resultValues << resultValue;
    }
    return resultValues;
}

std::pair<QString, QString> ParsingToolBox::finalScalarResult() const
{
    QString scalarText;
    QString lastScalarText;
    auto listDie= diceResultFromEachInstruction();
    if(hasIntegerResultNotInFirst())
    {
        QStringList strLst;
        auto listScalar= scalarResultsFromEachInstruction();
        for(auto val : listScalar)
        {
            strLst << QString::number(val);
        }
        scalarText= QString("%1").arg(strLst.join(','));
        lastScalarText= strLst.last();
    }
    else if(!listDie.isEmpty())
    {
        auto values= sumOfDiceResult();
        QStringList strLst;
        for(auto val : values)
        {
            strLst << QString::number(val);
        }
        scalarText= QString("%1").arg(strLst.join(','));
    }
    return {scalarText, lastScalarText};
}

bool ParsingToolBox::hasIntegerResultNotInFirst() const
{
    bool result= false;
    for(auto node : m_startNodes)
    {
        result|= hasResultOfType(Dice::RESULT_TYPE::SCALAR, node, true).first;
    }
    return result;
}

bool ParsingToolBox::hasDiceResult() const
{
    bool result= false;
    for(auto node : m_startNodes)
    {
        result|= hasResultOfType(Dice::RESULT_TYPE::DICE_LIST, node).first;
    }
    return result;
}
bool ParsingToolBox::hasStringResult() const
{
    bool result= false;
    for(auto node : m_startNodes)
    {
        result|= hasResultOfType(Dice::RESULT_TYPE::STRING, node).first;
    }
    return result;
}

QList<ExportedDiceResult> ParsingToolBox::diceResultFromEachInstruction() const
{
    QList<ExportedDiceResult> resultList;
    for(auto start : m_startNodes)
    {
        auto result= ParsingToolBox::finalDiceResultFromInstruction(start);
        resultList.append(result);
    }
    return resultList;
}

QStringList listOfDiceResult(const QList<ExportedDiceResult>& list)
{
    QStringList listOfDiceResult;
    for(auto map : list)
    {
        for(auto key : map.keys())
        {
            auto listOfList= map.value(key);
            for(auto dice : listOfList)
            {
                QString stringVal;
                for(auto val : dice)
                {
                    qint64 total= 0;
                    QStringList dicelist;
                    for(auto score : val.result())
                    {
                        total+= score;
                        dicelist << QString::number(score);
                    }
                    if(val.result().size() > 1)
                    {
                        stringVal= QString("%1 [%2]").arg(total).arg(dicelist.join(','));
                        listOfDiceResult << stringVal;
                    }
                    else
                    {
                        listOfDiceResult << QString::number(total);
                    }
                }
            }
        }
    }
    return listOfDiceResult;
}

QString ParsingToolBox::finalStringResult() const
{
    bool ok;
    QStringList allStringlist= allFirstResultAsString(ok);
    auto listFull= diceResultFromEachInstruction();

    QStringList resultWithPlaceHolder;
    std::for_each(allStringlist.begin(), allStringlist.end(), [&resultWithPlaceHolder](const QString& sub) {
        QRegularExpression ex("%[1-3]?|\\$[1-9]+|@[1-9]+");
        if(sub.contains(ex))
            resultWithPlaceHolder.append(sub);
    });
    auto stringResult= resultWithPlaceHolder.isEmpty() ? allStringlist.join(",") : resultWithPlaceHolder.join(",");

    auto pairScalar= finalScalarResult();

    stringResult.replace("%1", pairScalar.first);
    stringResult.replace("%2", listOfDiceResult(diceResultFromEachInstruction()).join(",").trimmed());
    stringResult.replace("%3", pairScalar.second);
    stringResult.replace("\\n", "\n");

    QMap<Dice::ERROR_CODE, QString> errorMap;
    stringResult= ParsingToolBox::replaceVariableToValue(stringResult, allStringlist, errorMap);
    stringResult= ParsingToolBox::replacePlaceHolderToValue(stringResult, listFull);

    return stringResult;
}

bool ParsingToolBox::readString(QString& str, QString& strResult)
{
    if(str.isEmpty())
        return false;

    if(str.startsWith('"'))
    {
        str= str.remove(0, 1);

        int i= 0;
        int j= 0;
        bool previousEscape= false;
        QString result;
        /*&&
                (((!previousEscape) && !(str[i]=='"')) || (previousEscape) && !(str[i]=='"'))
                    || (str[i]=='\\'))*/
        while(i < str.length() && (!(!previousEscape && (str[i] == '"')) || (previousEscape && str[i] != '"')))
        {
            if(str[i] == '\\')
            {
                previousEscape= true;
            }
            else
            {
                if(previousEscape && str[i] != '\"')
                {
                    result+= '\\';
                    ++j;
                }
                result+= str[i];
                previousEscape= false;
            }
            ++i;
        }

        if(!result.isEmpty())
        {
            str= str.remove(0, i);
            strResult= result;
            if(str.startsWith('"'))
            {
                str= str.remove(0, 1);
                return true;
            }
        }
    }

    return false;
}

bool ParsingToolBox::readVariable(QString& str, qint64& myNumber, QString& reasonFail)
{
    if(str.isEmpty())
        return false;

    if(str.startsWith("${"))
    {
        str= str.remove(0, 2);
    }
    QString key;
    int post= str.indexOf('}');
    key= str.left(post);

    if(!m_variableHash.isEmpty())
    {
        if(m_variableHash.contains(key))
        {
            QString value= m_variableHash.value(key);
            bool ok;
            int valueInt= value.toInt(&ok);
            if(ok)
            {
                myNumber= valueInt;
                str= str.remove(0, post + 1);
                return true;
            }
            else
            {
                reasonFail= QStringLiteral("Variable value is %1, not a number").arg(value);
            }
        }
        else
        {
            reasonFail= QStringLiteral("Variable not found");
        }
    }
    else
    {
        reasonFail= QStringLiteral("No Variables are defined");
    }

    return false;
}
bool ParsingToolBox::readComma(QString& str)
{
    if(str.startsWith(","))
    {
        str= str.remove(0, 1);
        return true;
    }
    else
        return false;
}
bool ParsingToolBox::readOpenParentheses(QString& str)
{
    if(str.startsWith("("))
    {
        str= str.remove(0, 1);
        return true;
    }
    else
        return false;
}

bool ParsingToolBox::readCloseParentheses(QString& str)
{
    if(str.startsWith(")"))
    {
        str= str.remove(0, 1);
        return true;
    }
    else
        return false;
}

int ParsingToolBox::findClosingCharacterIndexOf(QChar open, QChar closing, const QString& str, int offset)
{
    int counter= offset;
    int i= 0;
    for(auto const& letter : str)
    {
        if(letter == open)
            ++counter;
        else if(letter == closing)
            --counter;

        if(counter == 0)
            return i;

        ++i;
    }
    return -1;
}

bool ParsingToolBox::readList(QString& str, QStringList& list, QList<Range>& ranges)
{
    if(str.startsWith("["))
    {
        str= str.remove(0, 1);
        int pos= findClosingCharacterIndexOf('[', ']', str, 1); // str.indexOf("]");
        if(-1 != pos)
        {
            QString liststr= str.left(pos);
            list= liststr.split(",");
            str= str.remove(0, pos + 1);
            readProbability(list, ranges);
            return true;
        }
    }
    return false;
}
bool ParsingToolBox::readAscending(QString& str)
{
    if(str.isEmpty())
    {
        return false;
    }
    else if(str.at(0) == 'l')
    {
        str= str.remove(0, 1);
        return true;
    }
    return false;
}
Dice::CONDITION_STATE ParsingToolBox::isValidValidator(ExecutionNode* previous, ValidatorList* val)
{
    DiceRollerNode* node= getDiceRollerNode(previous);
    if(nullptr == node)
        return Dice::CONDITION_STATE::ERROR_STATE;

    return val->isValidRangeSize(node->getRange());
}
DiceRollerNode* ParsingToolBox::getDiceRollerNode(ExecutionNode* previous)
{
    while(nullptr != previous)
    {
        DiceRollerNode* node= dynamic_cast<DiceRollerNode*>(previous);
        if(nullptr != node)
        {
            return node;
        }
        previous= previous->getPreviousNode();
    }
    return nullptr;
}
bool ParsingToolBox::readDiceRange(QString& str, qint64& start, qint64& end)
{
    bool expectSquareBrasket= false;

    if((str.startsWith("[")))
    {
        str= str.remove(0, 1);
        expectSquareBrasket= true;
    }
    if(readNumber(str, start))
    {
        if(str.startsWith(".."))
        {
            str= str.remove(0, 2);
            if(readNumber(str, end))
            {
                if(expectSquareBrasket)
                {
                    if(str.startsWith("]"))
                    {
                        str= str.remove(0, 1);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
ParsingToolBox::LIST_OPERATOR ParsingToolBox::readListOperator(QString& str)
{
    if(str.startsWith('u'))
    {
        str= str.remove(0, 1);
        return UNIQUE;
    }
    return NONE;
}

bool ParsingToolBox::readPainterParameter(PainterNode* painter, QString& str)
{
    if(!str.startsWith('['))
        return false;

    str= str.remove(0, 1);
    int pos= str.indexOf(']');

    if(pos == -1)
        return false;

    QString data= str.left(pos);
    str= str.remove(0, pos + 1);
    QStringList duos= data.split(',');
    bool result= false;
    for(QString& duoStr : duos)
    {
        QStringList keyValu= duoStr.split(':');
        if(keyValu.size() == 2)
        {
            painter->insertColorItem(keyValu[1], keyValu[0].toInt());
            result= true;
        }
    }

    return result;
}

QHash<QString, QString> ParsingToolBox::getVariableHash()
{
    return m_variableHash;
}

void ParsingToolBox::setVariableHash(const QHash<QString, QString>& variableHash)
{
    m_variableHash= variableHash;
}

void ParsingToolBox::setStartNodes(std::vector<ExecutionNode*> nodes)
{
    m_startNodes= nodes;
}

void ParsingToolBox::readProbability(QStringList& str, QList<Range>& ranges)
{
    quint64 totalDistance= 0;
    quint64 undefDistance= 0;
    int undefCount= 0;
    int maxValue= 0;
    int i= 0;
    int j= 0;
    bool hasPercentage= false;
    for(QString line : str)
    {
        int pos= line.indexOf('[');
        if(-1 != pos)
        {
            QString rangeStr= line.right(line.length() - pos);
            line= line.left(pos);
            str[j]= line;
            qint64 start= 0;
            qint64 end= 0;
            if(readDiceRange(rangeStr, start, end))
            {
                Range range;
                range.setValue(start, end);
                ranges.append(range);
                totalDistance+= static_cast<quint64>(end - start + 1);
                ++i;
            }
            else // percentage
            {
                hasPercentage= true;
                Range range;
                range.setStart(start);
                ranges.append(range);
                ++undefCount;
                undefDistance+= static_cast<quint64>(start);
            }
            if((end > maxValue) || (i == 1))
            {
                maxValue= static_cast<int>(end);
            }
        }
        else
        {
            Range range;
            range.setEmptyRange(true);
            ranges.append(range);
        }
        ++j;
    }

    if((hasPercentage) && (undefDistance != 0))
    {
        qreal ratio= 100.0 / static_cast<qreal>(undefDistance);
        qint64 realStart= 0;
        for(int i= 0; i < ranges.size(); ++i)
        {
            Range tmp= ranges.at(i);
            if(!tmp.isFullyDefined())
            {
                int dist= static_cast<int>(tmp.getStart());
                tmp.setStart(realStart + 1);
                double truc= dist * ratio;

                tmp.setEnd(static_cast<int>(realStart + truc));
                realStart= tmp.getEnd();
                ranges[i]= tmp;
            }
        }
    }
    else
    {
        int limitUp= 1;
        for(int i= 0; i < ranges.size(); ++i)
        {
            Range range= ranges.at(i);
            if(range.isEmptyRange())
            {
                range.setStart(limitUp);
                range.setEnd(limitUp);
                range.setEmptyRange(false);
            }
            else
            {
                qint64 sizeRange= range.getEnd() - range.getStart();
                range.setStart(limitUp);
                limitUp+= sizeRange;
                range.setEnd(limitUp);
            }
            ++limitUp;
            ranges[i]= range;
        }
    }
}
bool ParsingToolBox::readComment(QString& str, QString& result, QString& comment)
{
    QString left= str;
    str= str.trimmed();
    if(str.startsWith("#"))
    {
        comment= left;
        str= str.remove(0, 1);
        result= str.trimmed();
        str= "";
        return true;
    }
    return false;
}

QString ParsingToolBox::replaceVariableToValue(const QString& source, QStringList values,
                                               QMap<Dice::ERROR_CODE, QString>& errorMap)
{
    QString result= source;

    int start= source.size() - 1;
    bool valid= true;
    do
    {
        auto ref= readVariableFromString(source, start);
        if(ref.resultIndex() > values.size())
        {
            auto error= QString("No valid value at index: $%1").arg(ref.resultIndex());
            errorMap.insert(Dice::ERROR_CODE::INVALID_INDEX, error);
            return error;
        }

        valid= ref.isValid();
        if(!valid)
            continue;

        result.remove(ref.position(), ref.length());
        auto val= values[ref.resultIndex() - 1];

        if(ref.subIndex() >= 0)
        {
            auto valSplit= val.split(",");
            if(ref.subIndex() < valSplit.size())
                val= valSplit[ref.subIndex()];
        }

        if(ref.digitNumber() != 0)
        {
            auto realVal= QString("%1").arg(val, ref.digitNumber(), QChar('0'));
            result.insert(ref.position(), realVal);
        }
        else
        {
            result.insert(ref.position(), val);
        }
    } while(valid);

    return result;
}

QString ParsingToolBox::replacePlaceHolderFromJson(const QString& source, const QJsonObject& obj)
{
    QStringList resultList;
    auto instructions= obj["instructions"].toArray();
    std::vector<std::vector<std::pair<int, QList<QStringList>>>> instructionResult;
    for(auto inst : instructions)
    {
        std::vector<std::pair<int, QList<QStringList>>> map;
        auto obj= inst.toObject();
        auto vals= obj["diceval"].toArray();
        int lastFace= -1;
        for(auto valRef : vals)
        {
            auto diceObj= valRef.toObject();
            auto face= diceObj["face"].toInt();
            auto it= std::find_if(std::begin(map), std::end(map),
                                  [face](const std::pair<int, QList<QStringList>>& val) { return val.first == face; });

            auto realVal= diceObj["string"].toString();
            if(lastFace == -1 || lastFace != face)
            {
                QList<QStringList> listOfList;
                listOfList << (QStringList() << realVal);
                map.push_back({face, listOfList});
            }
            else if(lastFace == face)
            {
                auto& valList= it->second.last();
                valList.append(realVal);
            }
            lastFace= face;
        }
        instructionResult.push_back(map);
    }
    std::transform(std::begin(instructionResult), std::end(instructionResult), std::back_inserter(resultList),
                   [](const std::vector<std::pair<int, QList<QStringList>>>& map) {
                       QStringList valuesStr;
                       auto multiKey= (map.size() > 1);
                       for(auto item : map)
                       {
                           auto face= item.first;
                           auto valueList= item.second;
                           QStringList strs;
                           for(auto list : valueList)
                           {
                               strs << list.join(",");
                           }
                           if(!multiKey)
                               valuesStr << strs.join(",");
                           else
                               valuesStr << QString("d%1:(%2)").arg(face).arg(strs.join(","));
                       }
                       return valuesStr.join(" - ");
                   });

    QString result= source;
    int start= source.size() - 1;
    bool valid= true;
    do
    {
        auto ref= readPlaceHolderFromString(source, start);
        if(ref.isValid())
        {
            result.remove(ref.position(), ref.length());
            auto val= resultList[ref.resultIndex() - 1];
            result.insert(ref.position(), val);
        }
        else
        {
            valid= false;
        }
    } while(valid);

    return result;
}

QString ParsingToolBox::replacePlaceHolderToValue(const QString& source, const QList<ExportedDiceResult>& list)
{
    QStringList resultList;
    std::transform(
        std::begin(list), std::end(list), std::back_inserter(resultList), [](const ExportedDiceResult& dice) {
            QStringList valuesStr;
            if(dice.size() == 1)
            {
                auto values= dice.values();
                std::transform(std::begin(values), std::end(values), std::back_inserter(valuesStr),
                               [](const QList<ListDiceResult>& dice) {
                                   QStringList textList;
                                   std::transform(std::begin(dice), std::end(dice), std::back_inserter(textList),
                                                  [](const ListDiceResult& dice) {
                                                      QStringList list;
                                                      std::transform(
                                                          std::begin(dice), std::end(dice), std::back_inserter(list),
                                                          [](const HighLightDice& hl) { return hl.getResultString(); });
                                                      return list.join(",");
                                                  });
                                   return textList.join(",");
                               });
            }
            else if(dice.size() > 1)
            {
                for(auto key : dice.keys())
                {
                    auto list= dice.value(key);
                    for(auto values : list)
                    {
                        QStringList textVals;
                        std::transform(std::begin(values), std::end(values), std::back_inserter(textVals),
                                       [](const HighLightDice& dice) { return dice.getResultString(); });
                        valuesStr.append(QString("d%1 [%2]").arg(key).arg(textVals.join(",")));
                    }
                }
            }
            return valuesStr.join(",");
        });

    QString result= source;
    int start= source.size() - 1;
    bool valid= true;
    do
    {
        auto ref= readPlaceHolderFromString(source, start);
        if(ref.isValid())
        {
            result.remove(ref.position(), ref.length());
            auto val= resultList[ref.resultIndex() - 1];
            result.insert(ref.position(), val);
        }
        else
        {
            valid= false;
        }
    } while(valid);

    return result;
}
void ParsingToolBox::readSubtitutionParameters(SubtituteInfo& info, QString& rest)
{
    auto sizeS= rest.size();
    if(rest.startsWith("{"))
    {
        rest= rest.remove(0, 1);
        qint64 number;
        if(readNumber(rest, number))
        {
            if(rest.startsWith("}"))
            {
                rest= rest.remove(0, 1);
                info.setDigitNumber(static_cast<int>(number));
            }
        }
    }
    if(rest.startsWith("["))
    {
        rest= rest.remove(0, 1);
        qint64 number;
        if(readNumber(rest, number))
        {
            if(rest.startsWith("]"))
            {
                rest= rest.remove(0, 1);
                info.setSubIndex(static_cast<int>(number));
            }
        }
    }
    info.setLength(info.length() + sizeS - rest.size());
}

bool ParsingToolBox::readReaperArguments(RepeaterNode* node, QString& source)
{
    if(!readOpenParentheses(source))
        return false;

    auto instructions= readInstructionList(source, false);
    if(instructions.empty())
        return false;

    readComma(source);
    ExecutionNode* tmp;
    if(readOperand(source, tmp))
    {
        if(source.startsWith("+"))
        {
            node->setSumAll(true);
            source= source.remove(0, 1);
        }
        if(readCloseParentheses(source))
        {
            node->setCommand(instructions);
            node->setTimeNode(tmp);
            return true;
        }
    }

    return false;
}
bool ParsingToolBox::readExpression(QString& str, ExecutionNode*& node)
{
    ExecutionNode* operandNode= nullptr;
    if(readOpenParentheses(str))
    {
        ExecutionNode* internalNode= nullptr;
        if(readExpression(str, internalNode))
        {
            ParenthesesNode* parentheseNode= new ParenthesesNode();
            parentheseNode->setInternelNode(internalNode);
            node= parentheseNode;
            if(readCloseParentheses(str))
            {
                ExecutionNode* diceNode= nullptr;
                ExecutionNode* operatorNode= nullptr;
                if(readDice(str, diceNode))
                {
                    parentheseNode->setNextNode(diceNode);
                }
                else if(readExpression(str, operatorNode))
                {
                    parentheseNode->setNextNode(operatorNode);
                }
                return true;
            }
            else
            {
                m_warningMap.insert(Dice::ERROR_CODE::BAD_SYNTAXE,
                                    QObject::tr("Expected closing parenthesis - can't validate the inside."));
            }
        }
    }
    else if(readFunction(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else if(readOptionFromNull(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else if(readOperatorFromNull(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else if(readOperand(str, operandNode))
    {
        ExecutionNode* diceNode= nullptr;
        if(readDice(str, diceNode))
        {
            operandNode->setNextNode(diceNode);
        }
        node= operandNode;

        operandNode= ParsingToolBox::getLatestNode(operandNode);
        // ExecutionNode* operatorNode=nullptr;
        while(readOperator(str, operandNode))
        {
            // operandNode->setNextNode(operatorNode);
            operandNode= ParsingToolBox::getLatestNode(operandNode);
        }
        return true;
    }
    else if(readCommand(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else if(readNode(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else if(readValuesList(str, operandNode))
    {
        node= operandNode;
        return true;
    }
    else
    {
        ExecutionNode* diceNode= nullptr;
        if(readDice(str, diceNode))
        {
            NumberNode* numberNode= new NumberNode();
            numberNode->setNumber(1);
            numberNode->setNextNode(diceNode);
            node= numberNode;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

bool ParsingToolBox::readValuesList(QString& str, ExecutionNode*& node)
{
    if(str.startsWith("["))
    {
        str= str.remove(0, 1);
        int pos= ParsingToolBox::findClosingCharacterIndexOf('[', ']', str, 1); // str.indexOf("]");
        if(-1 != pos)
        {
            QString liststr= str.left(pos);
            auto list= liststr.split(",");
            str= str.remove(0, pos + 1);
            auto values= new ValuesListNode();
            for(auto var : list)
            {
                qint64 number= 1;
                QString error;
                var= var.trimmed();
                if(ParsingToolBox::readDynamicVariable(var, number))
                {
                    VariableNode* variableNode= new VariableNode();
                    variableNode->setIndex(static_cast<quint64>(number - 1));
                    variableNode->setData(&m_startNodes);
                    values->insertValue(variableNode);
                }
                else if(ParsingToolBox::readNumber(var, number))
                {
                    NumberNode* numberNode= new NumberNode();
                    numberNode->setNumber(number);
                    values->insertValue(numberNode);
                }
            }
            node= values;
            return true;
        }
    }
    return false;
}
bool ParsingToolBox::readOptionFromNull(QString& str, ExecutionNode*& node)
{
    StartingNode nodePrevious;
    if(readOption(str, &nodePrevious))
    {
        auto nodeNext= nodePrevious.getNextNode();
        nodePrevious.setNextNode(nullptr);
        node= nodeNext;
        return true;
    }
    return false;
}

void ParsingToolBox::setHelpPath(const QString& path)
{
    m_helpPath= path;
}

bool ParsingToolBox::readOperatorFromNull(QString& str, ExecutionNode*& node)
{
    StartingNode nodePrevious;
    if(readOperator(str, &nodePrevious))
    {
        auto nodeNext= nodePrevious.getNextNode();
        nodePrevious.setNextNode(nullptr);
        node= nodeNext;
        return true;
    }
    return false;
}

bool ParsingToolBox::readOption(QString& str, ExecutionNode* previous) //,
{
    if(str.isEmpty())
    {
        return false;
    }

    ExecutionNode* node= nullptr;
    bool found= false;
    auto keys= m_OptionOp.keys();
    for(int i= 0; ((i < keys.size()) && (!found)); ++i)
    {
        QString key= keys.at(i);

        if(str.startsWith(key))
        {
            str= str.remove(0, key.size());
            auto operatorName= m_OptionOp.value(key);
            switch(operatorName)
            {
            case Keep:
            {
                qint64 myNumber= 0;
                bool ascending= readAscending(str);

                if(readNumber(str, myNumber))
                {
                    node= addSort(previous, ascending);
                    KeepDiceExecNode* nodeK= new KeepDiceExecNode();
                    nodeK->setDiceKeepNumber(myNumber);
                    node->setNextNode(nodeK);
                    node= nodeK;
                    found= true;
                }
            }
            break;
            case KeepAndExplode:
            {
                qint64 myNumber= 0;
                bool ascending= readAscending(str);
                if(readNumber(str, myNumber))
                {
                    /* if(!hasDice)
                        {
                            previous = addRollDiceNode(DEFAULT_FACES_NUMBER,previous);
                        }*/
                    DiceRollerNode* nodeTmp= dynamic_cast<DiceRollerNode*>(previous);
                    if(nullptr != nodeTmp)
                    {
                        previous= addExplodeDiceNode(static_cast<qint64>(nodeTmp->getFaces()), previous);
                    }

                    node= addSort(previous, ascending);

                    KeepDiceExecNode* nodeK= new KeepDiceExecNode();
                    nodeK->setDiceKeepNumber(myNumber);

                    node->setNextNode(nodeK);
                    node= nodeK;
                    found= true;
                }
            }
            break;
            case Filter:
            {
                auto validatorList= readValidatorList(str);
                if(nullptr != validatorList)
                {
                    auto validity= isValidValidator(previous, validatorList);

                    FilterNode* filterNode= new FilterNode();
                    filterNode->setValidatorList(validatorList);

                    previous->setNextNode(filterNode);
                    node= filterNode;
                    found= true;
                }
            }
            break;
            case Sort:
            {
                bool ascending= readAscending(str);
                node= addSort(previous, ascending);
                /*if(!hasDice)
                    {
                        m_errorMap.insert(ExecutionNode::BAD_SYNTAXE,QObject::tr("Sort Operator does not support default
                   dice. You should add dice command before the s"));
                    }*/
                found= true;
            }
            break;
            case Count:
            {
                auto validatorList= readValidatorList(str);
                if(nullptr != validatorList)
                {
                    auto validity= isValidValidator(previous, validatorList);

                    CountExecuteNode* countNode= new CountExecuteNode();
                    countNode->setValidatorList(validatorList);

                    previous->setNextNode(countNode);
                    node= countNode;
                    found= true;
                }
                else
                {
                    m_errorMap.insert(Dice::ERROR_CODE::BAD_SYNTAXE,
                                      QObject::tr("Validator is missing after the c operator. Please, change it"));
                }
            }
            break;
            case Reroll:
            case RerollUntil:
            case RerollAndAdd:
                // Todo: I think that Exploding and Rerolling could share the same code
                {
                    auto validatorList= readValidatorList(str);
                    QString symbol= m_OptionOp.key(operatorName);
                    if(nullptr != validatorList)
                    {
                        switch(isValidValidator(previous, validatorList))
                        {
                        case Dice::CONDITION_STATE::ALWAYSTRUE:
                            if(operatorName == RerollAndAdd)
                            {
                                m_errorMap.insert(
                                    Dice::ERROR_CODE::ENDLESS_LOOP_ERROR,
                                    QObject::tr("Validator is always true for the %1 operator. Please, change it")
                                        .arg(symbol));
                            }
                            break;
                        case Dice::CONDITION_STATE::UNREACHABLE:
                            if(operatorName == RerollUntil)
                            {
                                m_errorMap.insert(
                                    Dice::ERROR_CODE::ENDLESS_LOOP_ERROR,
                                    QObject::tr("Condition can't be reached, causing endless loop. Please, "
                                                "change the %1 option condition")
                                        .arg(symbol));
                            }
                            break;
                        case Dice::CONDITION_STATE::ERROR_STATE:
                        default:
                            break;
                        }

                        auto reroll= (operatorName == RerollAndAdd || operatorName == Reroll);
                        auto addingMode= (operatorName == RerollAndAdd);
                        RerollDiceNode* rerollNode= new RerollDiceNode(reroll, addingMode);
                        ExecutionNode* nodeParam= nullptr;
                        if(readParameterNode(str, nodeParam))
                        {
                            rerollNode->setInstruction(nodeParam);
                        }
                        rerollNode->setValidatorList(validatorList);
                        previous->setNextNode(rerollNode);
                        node= rerollNode;
                        found= true;
                    }
                    else
                    {
                        m_errorMap.insert(
                            Dice::ERROR_CODE::BAD_SYNTAXE,
                            QObject::tr("Validator is missing after the %1 operator. Please, change it").arg(symbol));
                    }
                }
                break;
            case Explode:
            {
                auto validatorList= readValidatorList(str);
                if(nullptr != validatorList)
                {
                    if(Dice::CONDITION_STATE::ALWAYSTRUE == isValidValidator(previous, validatorList))
                    {
                        m_errorMap.insert(Dice::ERROR_CODE::ENDLESS_LOOP_ERROR,
                                          QObject::tr("This condition %1 introduces an endless loop. Please, change it")
                                              .arg(validatorList->toString()));
                    }
                    ExplodeDiceNode* explodedNode= new ExplodeDiceNode();
                    explodedNode->setValidatorList(validatorList);
                    previous->setNextNode(explodedNode);
                    node= explodedNode;
                    found= true;
                }
                else
                {
                    m_errorMap.insert(Dice::ERROR_CODE::BAD_SYNTAXE,
                                      QObject::tr("Validator is missing after the e operator. Please, change it"));
                }
            }
            break;
            case Merge:
            {
                MergeNode* mergeNode= new MergeNode();
                mergeNode->setStartList(&m_startNodes);
                previous->setNextNode(mergeNode);
                node= mergeNode;
                found= true;
            }
            break;
            case AllSameExplode:
            {
                AllSameNode* allSame= new AllSameNode();
                previous->setNextNode(allSame);
                node= allSame;
                found= true;
            }
            break;
            case Bind:
            {
                BindNode* bindNode= new BindNode();
                bindNode->setStartList(&m_startNodes);
                previous->setNextNode(bindNode);
                node= bindNode;
                found= true;
            }
            break;
            case Occurences:
            {
                qint64 number= 0;
                auto occNode= new OccurenceCountNode();
                if(readNumber(str, number))
                {
                    occNode->setWidth(number);
                    auto validatorList= readValidatorList(str);
                    if(validatorList)
                    {
                        occNode->setValidatorList(validatorList);
                    }
                    else if(readComma(str))
                    {
                        if(readNumber(str, number))
                        {
                            occNode->setHeight(number);
                        }
                    }
                }
                previous->setNextNode(occNode);
                node= occNode;
                found= true;
            }
            break;
            case Unique:
            {
                node= new UniqueNode();
                previous->setNextNode(node);
                found= true;
            }
            break;
            case Painter:
            {
                PainterNode* painter= new PainterNode();
                if(!readPainterParameter(painter, str))
                {
                    m_errorMap.insert(Dice::ERROR_CODE::BAD_SYNTAXE,
                                      QObject::tr("Missing parameter for Painter node (p)"));
                    delete painter;
                }
                else
                {
                    previous->setNextNode(painter);
                    node= painter;
                    found= true;
                }
            }
            break;
            case ifOperator:
            {
                IfNode* nodeif= new IfNode();
                nodeif->setConditionType(readConditionType(str));
                auto validatorList= readValidatorList(str);
                if(nullptr != validatorList)
                {
                    ExecutionNode* trueNode= nullptr;
                    ExecutionNode* falseNode= nullptr;
                    if(readIfInstruction(str, trueNode, falseNode))
                    {
                        nodeif->setInstructionTrue(trueNode);
                        nodeif->setInstructionFalse(falseNode);
                        nodeif->setValidatorList(validatorList);
                        previous->setNextNode(nodeif);
                        node= nodeif;
                        found= true;
                    }
                    else
                    {
                        delete nodeif;
                    }
                }
                else
                {
                    delete nodeif;
                }
                break;
            }
            case Split:
            {
                SplitNode* splitnode= new SplitNode();
                previous->setNextNode(splitnode);
                node= splitnode;
                found= true;
            }
            break;
            case Group:
            {
                bool stringResult= readStringResultParameter(str);
                qint64 groupNumber= 0;
                if(readNumber(str, groupNumber))
                {
                    GroupNode* groupNode= new GroupNode(stringResult);
                    groupNode->setGroupValue(groupNumber);
                    previous->setNextNode(groupNode);
                    node= groupNode;
                    found= true;
                }
            }
            break;
            }
        }
    }
    return found;
}
bool ParsingToolBox::readStringResultParameter(QString& str)
{
    if(str.startsWith("s"))
    {
        str.remove(0, 1);
        return true;
    }
    return false;
}
bool ParsingToolBox::readIfInstruction(QString& str, ExecutionNode*& trueNode, ExecutionNode*& falseNode)
{
    if(readBlocInstruction(str, trueNode))
    {
        if(readBlocInstruction(str, falseNode))
        {
            return true;
        }
        return true;
    }
    return false;
}
DiceRollerNode* ParsingToolBox::addRollDiceNode(qint64 faces, ExecutionNode* previous)
{
    DiceRollerNode* mydiceRoller= new DiceRollerNode(faces);
    previous->setNextNode(mydiceRoller);
    return mydiceRoller;
}
ExplodeDiceNode* ParsingToolBox::addExplodeDiceNode(qint64 value, ExecutionNode* previous)
{
    ExplodeDiceNode* explodeDiceNode= new ExplodeDiceNode();
    NumberNode* node= new NumberNode();
    node->setNumber(value);
    BooleanCondition* condition= new BooleanCondition();
    condition->setConditionType(Dice::OnEach);
    condition->setValueNode(node);
    condition->setOperator(BooleanCondition::Equal);
    auto valList= new ValidatorList();
    valList->setValidators(QList<Validator*>() << condition);
    auto validity= isValidValidator(previous, valList);
    explodeDiceNode->setValidatorList(valList);
    previous->setNextNode(explodeDiceNode);
    return explodeDiceNode;
}
bool ParsingToolBox::readParameterNode(QString& str, ExecutionNode*& node)
{
    if(str.startsWith("("))
    {
        str= str.remove(0, 1);
        if(readExpression(str, node))
        {
            if(str.startsWith(")"))
            {
                str= str.remove(0, 1);
                return true;
            }
        }
    }
    return false;
}

bool ParsingToolBox::readBlocInstruction(QString& str, ExecutionNode*& resultnode)
{
    if(str.startsWith('{'))
    {
        str= str.remove(0, 1);
        ExecutionNode* node= nullptr;
        Die::ArithmeticOperator op;
        ScalarOperatorNode* scalarNode= nullptr;
        if(readArithmeticOperator(str, op))
        {
            scalarNode= new ScalarOperatorNode();
            scalarNode->setArithmeticOperator(op);
        }
        if(readExpression(str, node))
        {
            if(str.startsWith('}'))
            {
                if(nullptr == scalarNode)
                {
                    resultnode= node;
                }
                else
                {
                    resultnode= scalarNode;
                    scalarNode->setInternalNode(node);
                }
                str= str.remove(0, 1);
                return true;
            }
        }
    }
    return false;
}
bool ParsingToolBox::readDice(QString& str, ExecutionNode*& node)
{
    DiceOperator currentOperator;

    if(readDiceOperator(str, currentOperator))
    {
        if(currentOperator == D)
        {
            qint64 max;
            qint64 min;
            bool unique= (ParsingToolBox::UNIQUE == readListOperator(str)) ? true : false;
            Die::ArithmeticOperator op;

            bool hasOp= readArithmeticOperator(str, op);
            if(readNumber(str, max))
            {
                if(max < 1)
                {
                    m_errorMap.insert(
                        Dice::ERROR_CODE::BAD_SYNTAXE,
                        QObject::tr("Dice with %1 face(s) does not exist. Please, put a value higher than 0").arg(max));
                    return false;
                }
                DiceRollerNode* drNode= new DiceRollerNode(max);
                drNode->setUnique(unique);
                if(hasOp)
                {
                    drNode->setOperator(op);
                }
                node= drNode;
                ExecutionNode* current= drNode;
                while(readOption(str, current))
                {
                    current= ParsingToolBox::getLatestNode(current);
                }
                return true;
            }
            else if(readDiceRange(str, min, max))
            {
                DiceRollerNode* drNode= new DiceRollerNode(max, min);
                drNode->setUnique(unique);
                if(hasOp)
                {
                    drNode->setOperator(op);
                }
                node= drNode;
                ExecutionNode* current= drNode;
                while(readOption(str, current))
                {
                    current= ParsingToolBox::getLatestNode(current);
                }
                return true;
            }
        }
        else if(currentOperator == L)
        {
            QStringList list;
            QList<Range> listRange;
            ParsingToolBox::LIST_OPERATOR op= readListOperator(str);
            if(readList(str, list, listRange))
            {
                ListSetRollNode* lsrNode= new ListSetRollNode();
                lsrNode->setRangeList(listRange);
                if(op == ParsingToolBox::UNIQUE)
                {
                    lsrNode->setUnique(true);
                }
                lsrNode->setListValue(list);
                node= lsrNode;
                return true;
            }
            else
            {
                m_errorMap.insert(
                    Dice::ERROR_CODE::BAD_SYNTAXE,
                    QObject::tr(
                        "List is missing after the L operator. Please, add it (e.g : 1L[sword,spear,gun,arrow])"));
            }
        }
    }

    return false;
}
bool ParsingToolBox::readDiceOperator(QString& str, DiceOperator& op)
{
    QStringList listKey= m_mapDiceOp.keys();
    for(const QString& key : listKey)
    {
        if(str.startsWith(key, Qt::CaseInsensitive))
        {
            str= str.remove(0, key.size());
            op= m_mapDiceOp.value(key);
            return true;
        }
    }
    return false;
}
QString ParsingToolBox::convertAlias(QString str)
{
    for(auto& cmd : m_aliasList)
    {
        if(cmd->isEnable())
        {
            cmd->resolved(str);
        }
    }
    return str;
}

bool ParsingToolBox::readCommand(QString& str, ExecutionNode*& node)
{
    if(m_commandList.contains(str))
    {
        if(str == QLatin1String("help"))
        {
            str= str.remove(0, QLatin1String("help").size());
            HelpNode* help= new HelpNode();
            if(!m_helpPath.isEmpty())
            {
                help->setHelpPath(m_helpPath);
            }
            node= help;
        }
        else if(str == QLatin1String("la"))
        {
            str= str.remove(0, QLatin1String("la").size());
            node= new ListAliasNode(m_aliasList);
        }
        return true;
    }
    return false;
}

bool ParsingToolBox::readDiceExpression(QString& str, ExecutionNode*& node)
{
    bool returnVal= false;

    ExecutionNode* next= nullptr;
    if(readDice(str, next))
    {
        ExecutionNode* latest= next;
        while(readOption(str, latest))
        {
            while(nullptr != latest->getNextNode())
            {
                latest= latest->getNextNode();
            }
        }

        node= next;
        returnVal= true;
    }
    else
    {
        returnVal= false;
    }
    return returnVal;
}

bool ParsingToolBox::readOperator(QString& str, ExecutionNode* previous)
{
    if(str.isEmpty() || nullptr == previous)
    {
        return false;
    }

    Die::ArithmeticOperator op;
    if(readArithmeticOperator(str, op))
    {
        ScalarOperatorNode* node= new ScalarOperatorNode();
        node->setArithmeticOperator(op);
        ExecutionNode* nodeExec= nullptr;
        if(readExpression(str, nodeExec))
        {
            node->setInternalNode(nodeExec);
            if(nullptr == nodeExec)
            {
                delete node;
                return false;
            }
            ExecutionNode* nodeExecOrChild= nodeExec;
            ExecutionNode* parent= nullptr;

            while((nullptr != nodeExecOrChild) && (node->getPriority() < nodeExecOrChild->getPriority()))
            {
                parent= nodeExecOrChild;
                nodeExecOrChild= nodeExecOrChild->getNextNode();
            }
            // management of operator priority
            if((nullptr != nodeExecOrChild) && (nodeExec != nodeExecOrChild))
            {
                // good 1 1 2 ; bad 1 0 4
                if(nodeExecOrChild->getPriority() >= node->getPriority())
                {
                    node->setNextNode(nodeExecOrChild);
                    parent->setNextNode(nullptr);
                }
            }
            else if(node->getPriority() >= nodeExec->getPriority())
            {
                node->setNextNode(nodeExec->getNextNode());
                nodeExec->setNextNode(nullptr);
            }

            // nodeResult = node;
            previous->setNextNode(node);

            return true;
        }
        else
        {
            delete node;
        }
    }
    else
    {
        while(readOption(str, previous))
        {
            previous= ParsingToolBox::getLatestNode(previous);
        }
    }
    return false;
}
bool ParsingToolBox::readFunction(QString& str, ExecutionNode*& node)
{
    for(const auto& kv : m_functionMap)
    {
        if(str.startsWith(kv.first))
        {
            str= str.remove(0, kv.first.size());
            switch(kv.second)
            {
            case REPEAT:
            {
                auto repeaterNode= new RepeaterNode();
                if(ParsingToolBox::readReaperArguments(repeaterNode, str))
                {
                    node= repeaterNode;
                }
            }
            break;
            }
        }
    }

    if(node == nullptr)
        return false;
    return true;
}

bool ParsingToolBox::readNode(QString& str, ExecutionNode*& node)
{
    if(str.isEmpty())
        return false;

    QString key= str.at(0);
    if(m_nodeActionMap.contains(key))
    {
        JumpBackwardNode* jumpNode= new JumpBackwardNode();
        node= jumpNode;
        str= str.remove(0, 1);
        readOption(str, jumpNode);
        return true;
    }
    return false;
}

bool ParsingToolBox::readInstructionOperator(QChar c)
{
    if(c == ';')
    {
        return true;
    }
    return false;
}

void ParsingToolBox::insertAlias(DiceAlias* dice, int i)
{
    if(i >= m_aliasList.size())
    {
        m_aliasList.insert(i, dice);
    }
}
const QList<DiceAlias*>& ParsingToolBox::getAliases() const
{
    return m_aliasList;
}

QList<DiceAlias*>* ParsingToolBox::aliases()
{
    return &m_aliasList;
}

std::vector<ExecutionNode*> ParsingToolBox::readInstructionList(QString& str, bool global)
{
    if(str.isEmpty())
        return {};

    std::vector<ExecutionNode*> startNodes;

    bool hasInstruction= false;
    bool readInstruction= true;
    while(readInstruction)
    {
        ExecutionNode* startNode= nullptr;
        bool keepParsing= readExpression(str, startNode);
        if(nullptr != startNode)
        {
            hasInstruction= true;
            startNodes.push_back(startNode);
            auto latest= startNode;
            if(keepParsing)
            {
                latest= ParsingToolBox::getLatestNode(latest);
                keepParsing= !str.isEmpty();
                while(keepParsing)
                {
                    auto before= str;
                    if(readOperator(str, latest))
                    {
                        latest= ParsingToolBox::getLatestNode(latest);
                    }
                    keepParsing= (!str.isEmpty() && (before != str));
                }
            }
            if(!str.isEmpty() && readInstructionOperator(str[0]))
            {
                str= str.remove(0, 1);
            }
            else
            {
                QString result;
                QString comment;
                if(readComment(str, result, comment))
                {
                    m_comment= result;
                }
                readInstruction= false;
            }
        }
        else
        {
            readInstruction= false;
        }
    }
    if(global)
        m_startNodes= startNodes;
    return startNodes;
}

SubtituteInfo ParsingToolBox::readVariableFromString(const QString& source, int& start)
{
    bool found= false;
    SubtituteInfo info;
    int i= start;
    for(; i >= 0 && !found; --i)
    {
        if(source.at(i) == '$')
        {
            auto rest= source.mid(i + 1, 1 + start - i);
            qint64 number;
            if(readNumber(rest, number))
            {
                auto len= QString::number(number).size() - 1;
                readSubtitutionParameters(info, rest);
                info.setLength(info.length() + len);
                info.setResultIndex(static_cast<int>(number));
                info.setPosition(i);
                found= true;
            }
        }
    }
    start= i;
    return info;
}

SubtituteInfo ParsingToolBox::readPlaceHolderFromString(const QString& source, int& start)
{
    bool found= false;
    SubtituteInfo info;
    int i= start;
    for(; i >= 0 && !found; --i)
    {
        if(source.at(i) == '@')
        {
            auto rest= source.mid(i + 1, 1 + start - i);
            qint64 number;
            if(readNumber(rest, number))
            {
                auto len= QString::number(number).size() - 1;
                readSubtitutionParameters(info, rest);
                info.setLength(info.length() + len);
                info.setResultIndex(static_cast<int>(number));
                info.setPosition(i);
                found= true;
            }
        }
    }
    start= i;
    return info;
}

ExportedDiceResult ParsingToolBox::finalDiceResultFromInstruction(ExecutionNode* start)
{
    ExecutionNode* next= ParsingToolBox::getLeafNode(start);
    Result* result= next->getResult();
    ExportedDiceResult nodeResult;
    std::set<QString> alreadyAdded;
    while(nullptr != result)
    {
        if(result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
        {
            DiceResult* diceResult= dynamic_cast<DiceResult*>(result);
            QList<HighLightDice> list;
            quint64 faces= 0;
            for(auto& die : diceResult->getResultList())
            {
                faces= die->getFaces();
                HighLightDice hlDice(die->getListValue(), die->isHighlighted(), die->getColor(),
                                     die->hasBeenDisplayed(), die->getFaces(), die->getUuid());
                if(alreadyAdded.find(die->getUuid()) == alreadyAdded.end() && !hlDice.displayed())
                {
                    list.append(hlDice);
                    alreadyAdded.insert(die->getUuid());
                }
            }
            if(!list.isEmpty())
            {
                auto vals= nodeResult.value(faces);
                vals.append(list);
                nodeResult.insert(faces, vals);
            }
        }
        /*if(nodeResult.isEmpty())
            result= result->getPrevious();
        else*/
        result= result->getPrevious();
    }
    return nodeResult;
}

ExportedDiceResult ParsingToolBox::allDiceResultFromInstruction(ExecutionNode* start)
{
    ExecutionNode* next= ParsingToolBox::getLeafNode(start);
    Result* result= next->getResult();
    ExportedDiceResult nodeResult;
    std::set<QString> alreadyAdded;
    while(nullptr != result)
    {
        if(result->hasResultOfType(Dice::RESULT_TYPE::DICE_LIST))
        {
            DiceResult* diceResult= dynamic_cast<DiceResult*>(result);
            QList<HighLightDice> list;
            quint64 faces= 0;
            for(auto& die : diceResult->getResultList())
            {
                faces= die->getFaces();
                HighLightDice hlDice(die->getListValue(), die->isHighlighted(), die->getColor(),
                                     die->hasBeenDisplayed(), die->getFaces(), die->getUuid());
                if(alreadyAdded.find(die->getUuid()) == alreadyAdded.end())
                {
                    list.append(hlDice);
                    alreadyAdded.insert(die->getUuid());
                }
            }
            if(!list.isEmpty())
            {
                auto vals= nodeResult.value(faces);
                vals.append(list);
                nodeResult.insert(faces, vals);
            }
        }
        result= result->getPrevious();
    }
    return nodeResult;
}

void ParsingToolBox::addResultInJson(QJsonObject& obj, Dice::RESULT_TYPE type, const QString& key, ExecutionNode* start,
                                     bool b)
{
    auto pair= hasResultOfType(type, start, b);
    if(pair.first)
        obj[key]= QJsonValue::fromVariant(pair.second);
}

void ParsingToolBox::addDiceResultInJson(
    QJsonObject& obj, ExecutionNode* start,
    std::function<QString(const QString& value, const QString& color, bool highlighted)> colorize)
{
    QJsonArray diceValues;
    auto result= ParsingToolBox::allDiceResultFromInstruction(start);
    for(auto listOfList : result.values())
    {
        for(auto listDiceResult : listOfList)
        {
            for(auto hlDice : listDiceResult)
            {
                QJsonObject diceObj;
                diceObj["face"]= static_cast<qreal>(hlDice.faces());
                diceObj["color"]= hlDice.color();
                diceObj["displayed"]= hlDice.displayed();
                diceObj["string"]= colorize(hlDice.getResultString(), hlDice.color(), hlDice.isHighlighted());
                diceObj["highlight"]= hlDice.isHighlighted();
                diceObj["uuid"]= hlDice.uuid();
                auto val= hlDice.result();
                if(!val.isEmpty())
                {
                    diceObj["value"]= std::accumulate(val.begin(), val.end(), 0);
                    if(val.size() > 1)
                    {
                        QJsonArray intValues;
                        std::transform(val.begin(), val.end(), std::back_inserter(intValues),
                                       [](qint64 val) { return static_cast<int>(val); });
                        diceObj["subvalues"]= intValues;
                    }
                }
                diceValues.append(diceObj);
            }
        }
    }
    if(!diceValues.isEmpty())
        obj["diceval"]= diceValues;
}

SubtituteInfo::SubtituteInfo() {}

bool SubtituteInfo::isValid() const
{
    return !(m_position + m_resultIndex < 0);
}

int SubtituteInfo::length() const
{
    return m_length;
}

void SubtituteInfo::setLength(int length)
{
    m_length= length;
}

int SubtituteInfo::resultIndex() const
{
    return m_resultIndex;
}

void SubtituteInfo::setResultIndex(int valueIndex)
{
    m_resultIndex= valueIndex;
}

int SubtituteInfo::position() const
{
    return m_position;
}

void SubtituteInfo::setPosition(int position)
{
    m_position= position;
}

int SubtituteInfo::digitNumber() const
{
    return m_digitNumber;
}

void SubtituteInfo::setDigitNumber(int digitNumber)
{
    m_digitNumber= digitNumber;
}

int SubtituteInfo::subIndex() const
{
    return m_subIndex;
}

void SubtituteInfo::setSubIndex(int subindex)
{
    m_subIndex= subindex;
}
