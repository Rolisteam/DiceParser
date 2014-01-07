#include "diceparser.h"
#include <QDebug>
#include <QStringList>
#include <QObject>

#include "node/dicerollernode.h"
#include "node/startingnode.h"
#include "node/scalaroperatornode.h"
#include "node/numbernode.h"
#include "node/keepdiceexecnode.h"
#include "node/sortresult.h"
#include "node/countexecutenode.h"

DiceParser::DiceParser()
{
    m_mapDiceOp = new QMap<QString,DiceOperator>();
    m_mapDiceOp->insert("D",D);

    m_OptionOp = new QMap<QString,OptionOperator>();
    m_OptionOp->insert(QObject::tr("k"),keep);
    m_OptionOp->insert(QObject::tr("K"),KeepAndReRoll);
    m_OptionOp->insert(QObject::tr("s"),Sort);
    m_OptionOp->insert(QObject::tr("c"),Count);



    m_logicOp = new QMap<QString,BooleanCondition::LogicOperator>();
    m_logicOp->insert("<",BooleanCondition::LesserThan);
    m_logicOp->insert("=",BooleanCondition::Equal);
    m_logicOp->insert("<=",BooleanCondition::LesserOrEqual);
    m_logicOp->insert(">",BooleanCondition::GreaterThan);
    m_logicOp->insert(">=",BooleanCondition::GreaterOrEqual);




}

void DiceParser::setCurrentNode(ExecutionNode* node)
{
    ExecutionNode* next = node;
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
    }
    m_current = next;
}

void DiceParser::parseLine(QString str)
{
    QString command = str;
    m_start = new StartingNode();
    m_current = m_start;
    bool keepParsing = true;
    ExecutionNode* execNode=NULL;
    keepParsing = readDiceExpression(str,execNode);

    m_current->setNextNode(execNode);
    setCurrentNode(execNode);

    execNode=NULL;
    keepParsing =!str.isEmpty();
    while(keepParsing)
    {
        keepParsing = readOperator(str);
        //keepParsing = readOption(str);
    }

    m_start->run();
    ExecutionNode* next = m_start;
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
    }





    //////////////////////////////////
    //
    //  Display
    //
    //////////////////////////////////

    if(next->getResult()->isScalar())
    {
        qDebug() << "you get " << next->getResult()->getScalar() << " and you roll:" << command;
    }
    else
    {
        DiceResult* myDiceResult = static_cast<DiceResult*>(next->getResult());
        if(NULL==myDiceResult)
        {

            QString resulStr="{";
            foreach(Die die, myDiceResult->getResultList())
            {
                resulStr="[";
                foreach(qint64 i, die.getListValue())
                {
                    resulStr+=QString("%1,").arg(i);
                }
                resulStr+="]";
            }
            resulStr.remove(resulStr.size()-1,1);
            resulStr+="}";


            qDebug() << "you get " << resulStr  << " and you roll:" << command;
        }
    }
    //qDebug() << "list:" << << " sum:" << << " command:" << command;
}
bool DiceParser::readNumber(QString& str, int& myNumber)
{
    if(str.isEmpty())
        return false;

    QString number;
    int i=0;

    while(i<str.length() && str[i].isNumber())
    {
        number+=str[i];
        ++i;
    }

    if(number.isEmpty())
        return false;

    bool ok;
    myNumber = number.toInt(&ok);
    if(ok)
    {
        str=str.remove(0,number.size());
        return true;
    }
    return false;
}

bool DiceParser::readDice(QString&  str,Dice& dice)
{
    DiceOperator myOperator;
    if(readDiceOperator(str,myOperator))
    {
        int num;
        if(readNumber(str,num))
        {
            dice.m_diceOp = myOperator;
            dice.m_faces = num;
            return true;
        }
    }

    return false;

}
bool DiceParser::readDiceOperator(QString& str,DiceOperator& op)
{
    QStringList listKey = m_mapDiceOp->keys();
    foreach(QString key, listKey)
    {
        if(str.startsWith(key,Qt::CaseInsensitive))
        {
            str=str.remove(0,key.size());
            op = m_mapDiceOp->value(key);
            return true;
        }
    }
    return false;
}
bool DiceParser::readDiceExpression(QString& str,ExecutionNode* & node)
{
    int number=1;
    bool hasRead = readNumber(str,number);

    NumberNode* numberNode = new NumberNode();
    numberNode->setNumber(number);


    Dice myDice;
    if(readDice(str,myDice))
    {
        DiceRollerNode* next = new DiceRollerNode(myDice.m_faces);

        numberNode->setNextNode(next);

        node = numberNode;

        while(readOption(str));


        return true;
    }
    else if(hasRead)
    {
        node = numberNode;
        return true;
    }
    else
    {
        qDebug() << "error" << number << str;
        return false;
    }

}
bool DiceParser::readOperator(QString& str)
{
    if(str.isEmpty())
    {
        return false;
    }

    ScalarOperatorNode* node = new ScalarOperatorNode();
    if(node->setOperatorChar(str[0]))
    {

        ExecutionNode* nodeExec = NULL;
        str=str.remove(0,1);//removal of one character
        if(readDiceExpression(str,nodeExec))
        {

            node->setInternalNode(nodeExec);
            m_current->setNextNode(node);
            m_current= node;

            return true;
        }
    }
    return false;
}
bool DiceParser::readOption(QString& str)
{


    if(str.isEmpty())
    {
        return false;
    }



    foreach(QString tmp, m_OptionOp->keys())
    {

        if(str.startsWith(tmp))
        {

            str=str.remove(0,tmp.size());
            int myNumber=0;
            switch(m_OptionOp->value(tmp))
            {
                case KeepAndReRoll:
                {
                    if(readNumber(str,myNumber))
                    {
                        addSort(false);

                        KeepDiceExecNode* nodeK = new KeepDiceExecNode();
                        nodeK->setDiceKeepNumber(myNumber);

                        m_current->setNextNode(nodeK);
                        m_current= nodeK;
                        return true;

                    }
                }
                    break;
                case Sort:
                {
                    addSort(false);
                    return true;
                }
                    break;
                case Count:
                {
                    Validator* validator = readValidator(str);
                    if(NULL!=validator)
                    {
                        CountExecuteNode* node = new CountExecuteNode();
                        node->setValidator(validator);

                        m_current->setNextNode(node);
                        m_current = node;
                        return true;
                    }
                }
                    break;
            }
        }
    }
    return false;
}
Validator* DiceParser::readValidator(QString& str)
{
    Validator* returnVal=NULL;
    if(str.startsWith("["))
    {
        str=str.remove(0,1);
        int start=0;
        BooleanCondition::LogicOperator myLogicOp;
        if(readNumber(str,start))
        {
            if(str.startsWith("-"))
            {
                str=str.remove(0,1);
                int end=0;
                if(readNumber(str,end))
                {
                    if(str.startsWith("]"))
                    {
                        str=str.remove(0,1);
                        Range* range = new Range();
                        range->setValue(start,end);
                        returnVal = range;

                    }
                }
            }
        }
        else if(readLogicOperator(str,myLogicOp))
        {
            int value=0;
            if(readNumber(str,value))
            {
                BooleanCondition* condition = new BooleanCondition();
                condition->setValue(value);
                condition->setOperator(myLogicOp);
                returnVal = condition;

            }
        }
    }
    return returnVal;
}
bool DiceParser::readLogicOperator(QString& str,BooleanCondition::LogicOperator& op)
{
    foreach(QString tmp, m_logicOp->keys())
    {
        if(str.startsWith(tmp))
        {
           str=str.remove(0,tmp.size());
           op = m_logicOp->value(tmp);
           return true;
        }
    }
    return false;
}

void DiceParser::addSort(bool b)
{
    SortResultNode* nodeSort = new SortResultNode();
    nodeSort->setSortAscending(b);
    m_current->setNextNode(nodeSort);
    m_current = nodeSort;

}
