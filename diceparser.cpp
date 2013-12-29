#include "diceparser.h"
#include <QDebug>
#include <QStringList>

#include "node/dicerollernode.h"
#include "node/startingnode.h"
#include "node/scalaroperatornode.h"
#include "node/numbernode.h"

DiceParser::DiceParser()
{
    m_mapDiceOp = new QMap<QString,DiceOperator>();
    m_mapDiceOp->insert("D",D);
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

//      if(keepParsing)
//      {
//          keepParsing = readDiceExpression(str,execNode);

//          m_current->setNextNode(execNode);
//          m_current= execNode;

//          if(keepParsing)
//          {
//            keepParsing =!str.isEmpty();
//          }
//      }
    }

    m_start->run();
    ExecutionNode* next = m_start;
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
    }
    qDebug() << "list:" <<next->getResult()->getResultList() << "sum" <<next->getResult()->getSum() ;
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
