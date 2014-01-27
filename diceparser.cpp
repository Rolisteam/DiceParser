#include "diceparser.h"
#include <QDebug>
#include <QStringList>
#include <QObject>


#include "node/startingnode.h"
#include "node/scalaroperatornode.h"
#include "node/numbernode.h"
#include "node/keepdiceexecnode.h"
#include "node/sortresult.h"
#include "node/countexecutenode.h"
#include "node/rerolldicenode.h"
#include "node/explosedicenode.h"
#include "node/parenthesesnode.h"

DiceParser::DiceParser()
{
    m_parsingToolbox = new ParsingToolBox();

    m_mapDiceOp = new QMap<QString,DiceOperator>();
    m_mapDiceOp->insert("D",D);

    m_OptionOp = new QMap<QString,OptionOperator>();
    m_OptionOp->insert(QObject::tr("k"),keep);
    m_OptionOp->insert(QObject::tr("K"),KeepAndExplose);
    m_OptionOp->insert(QObject::tr("s"),Sort);
    m_OptionOp->insert(QObject::tr("c"),Count);
    m_OptionOp->insert(QObject::tr("r"),Reroll);
    m_OptionOp->insert(QObject::tr("e"),Explosing);


    m_aliasMap = new QMap<QString,QString>;
    m_aliasMap->insert("l5r","D10k");
    m_aliasMap->insert("l5R","D10e10k");
    m_aliasMap->insert("nwod","D10e10c[>7]");
    m_aliasMap->insert("nwod","D10e10c[>7]");
}

ExecutionNode* DiceParser::getLatestNode(ExecutionNode* node)
{
    ExecutionNode* next = node;
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
    }
    return next;
}

bool DiceParser::parseLine(QString str)
{
    m_command = str;
    m_start = new StartingNode();
    ExecutionNode* newNode = NULL;
    m_current = m_start;

    bool keepParsing = readExpression(str,newNode);


    while(keepParsing)
    {
        m_current->setNextNode(newNode);
        m_current = getLatestNode(m_current);
        keepParsing =!str.isEmpty();
        if(keepParsing)
        {
            keepParsing = readOperator(str,m_current);

            m_current = getLatestNode(m_current);
        }
        return true;
    }
    return false;

}

bool DiceParser::readExpression(QString& str,ExecutionNode* & node)
{
    //    int myNumber = 1;
    //    bool hasReadNumber=false;
    ExecutionNode* operandNode=NULL;
    if(m_parsingToolbox->readOpenParentheses(str))
    {
        ExecutionNode* internalNode=NULL;
        if(readExpression(str,internalNode))
        {
            ParenthesesNode* parentheseNode  = new ParenthesesNode();
            parentheseNode->setInternelNode(internalNode);
            node = parentheseNode;
            if(m_parsingToolbox->readCloseParentheses(str))
            {

                ExecutionNode* diceNode=NULL;
                if(readDice(str,diceNode))
                {
                    parentheseNode->setNextNode(diceNode);
                }

                return true;
            }


        }
    }
    else if(readOperand(str,operandNode))
    {
        ExecutionNode* diceNode=NULL;
        if(readDice(str,diceNode))
        {
            operandNode->setNextNode(diceNode);
        }
        node = operandNode;

        operandNode= getLatestNode(operandNode);
        while(readOperator(str,operandNode));



    }
    else
    {
         ExecutionNode* diceNode=NULL;
        if(readDice(str,diceNode))
        {
            NumberNode* numberNode=new NumberNode();
            numberNode->setNumber(1);
            numberNode->setNextNode(diceNode);
            node = numberNode;
        }
    }
    return true;
}

void DiceParser::Start()
{
    m_start->run();
}
void DiceParser::displayResult()
{

    ExecutionNode* next = m_start;
    int nodeCount=0;
    int resulCount=0;
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
        ++nodeCount;
    }
//    qDebug() << "node count"<< nodeCount;
    //////////////////////////////////
    //
    //  Display
    //
    //////////////////////////////////

    QString str;
    QTextStream stream(&str);
    Result* myResult=next->getResult();

    QString totalValue("you get %1 ;");
    QString dieValue("D%1 : {%2} ");

    bool scalarDone=false;
    while(NULL!=myResult)
    {
        ++resulCount;
        if((myResult->isScalar())&&(!scalarDone))
        {
            stream << totalValue.arg(myResult->getScalar()) << endl; //.arg(m_command)
            scalarDone=true;
        }

        DiceResult* myDiceResult = dynamic_cast<DiceResult*>(myResult);
        if(NULL!=myDiceResult)
        {

            QString resulStr;
            quint64 face=0;
            foreach(Die* die, myDiceResult->getResultList())
            {
                if(!die->hasBeenDisplayed())
                {
                    resulStr+=QString("%1").arg(die->getValue());
                    die->displayed();
                    face = die->getFaces();


                    if(die->hasChildrenValue())
                    {
                        resulStr+=" [";
                        foreach(qint64 i, die->getListValue())
                        {

                            resulStr+=QString("%1 ").arg(i);
                        }
                        resulStr.remove(resulStr.size()-1,1);
                        resulStr+="]";
                    }
                    resulStr+=", ";
                }
            }
            resulStr.remove(resulStr.size()-2,2);

            if(!resulStr.isEmpty())
            {
                stream << dieValue.arg(face).arg(resulStr);
            }

        }

        myResult = myResult->getPrevious();
    }

    QTextStream out(stdout);
    out << str << "you rolled: " <<m_command << endl;
    out <<  endl;

//    qDebug() << "result count:" << resulCount;
}



bool DiceParser::readDice(QString&  str,ExecutionNode* & node)
{
    DiceOperator myOperator;

    if(readDiceOperator(str,myOperator))
    {
        int num;
        if(m_parsingToolbox->readNumber(str,num))
        {
            DiceRollerNode* drNode = new DiceRollerNode(num);
            //            dice.m_diceOp = myOperator;
            //            dice.m_faces = num;
            node = drNode;
            ExecutionNode* current = drNode;
            while(readOption(str,current))
            {
                current = getLatestNode(current);
            }


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
    bool returnVal=false;


    /*ExecutionNode* operandeNode = NULL;
        if(readExpression(str,operandeNode))
        {*/
    // bool hasRead = m_parsingToolbox->readNumber(str,number);




    /*NumberNode* numberNode = new NumberNode();
            numberNode->setNumber(number);*/



    ExecutionNode* next = NULL;
    if(readDice(str,next))
    {
        //DiceRollerNode* next = addRollDiceNode(myDice.m_faces,operandeNode);
        //numberNode->setNextNode(next); already done in addRollDiceNode method




        ExecutionNode* latest = next;
        while(readOption(str,latest))
        {
            while(NULL!=latest->getNextNode())
            {
                latest = latest->getNextNode();
            }
        }

        node = next;
        returnVal = true;
    }
    /*    else if(NULL!=operandeNode)
            {
                //setCurrentNode(numberNode);
                returnVal = true;
                ExecutionNode* latest = operandeNode;
                while(readOption(str,latest,false))
                {
                    while(NULL!=latest->getNextNode())
                    {
                        latest = latest->getNextNode();
                    }
                }
            }*/
    else
    {
        qDebug() << "error" << number << str;
        returnVal = false;
    }

    //  }


    return returnVal;
}
bool DiceParser::readOperator(QString& str,ExecutionNode* previous)
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
        if(readExpression(str,nodeExec))
        {

            node->setInternalNode(nodeExec);
            previous->setNextNode(node);
            //node = getLatestNode(node);


            return true;
        }
    }
    return false;
}
DiceRollerNode* DiceParser::addRollDiceNode(qint64 faces,ExecutionNode* previous)
{
    DiceRollerNode* mydiceRoller= new DiceRollerNode(faces);
    previous->setNextNode(mydiceRoller);
    return mydiceRoller;
}

bool DiceParser::readOption(QString& str,ExecutionNode* previous, bool hasDice)
{


    if(str.isEmpty())
    {
        return false;
    }

    ExecutionNode* node = NULL;
    bool isFine=false;



    for(int i = 0; ((i<m_OptionOp->keys().size())&&(!isFine));++i )
    {
        QString tmp =m_OptionOp->keys().at(i);

        if(str.startsWith(tmp))
        {

            str=str.remove(0,tmp.size());

            switch(m_OptionOp->value(tmp))
            {
            case keep:
            {
                int myNumber=0;
                if(m_parsingToolbox->readNumber(str,myNumber))
                {
                    if(!hasDice)
                    {
                        previous = addRollDiceNode(10,previous);
                    }


                    node = m_parsingToolbox->addSort(previous,false);

                    KeepDiceExecNode* nodeK = new KeepDiceExecNode();
                    nodeK->setDiceKeepNumber(myNumber);

                    node->setNextNode(nodeK);
                    node = nodeK;
                    isFine = true;

                }
            }
                break;
            case KeepAndExplose:
            {
                int myNumber=0;
                if(m_parsingToolbox->readNumber(str,myNumber))
                {
                    if(!hasDice)
                    {
                        previous = addRollDiceNode(10,previous);
                    }


                    node = m_parsingToolbox->addSort(previous,false);

                    KeepDiceExecNode* nodeK = new KeepDiceExecNode();
                    nodeK->setDiceKeepNumber(myNumber);

                    node->setNextNode(nodeK);
                    node = nodeK;
                    isFine = true;

                }
            }
                break;
                break;
            case Sort:
            {
                node = m_parsingToolbox->addSort(previous,false);

                isFine = true;
            }
                break;
            case Count:
            {
                Validator* validator = m_parsingToolbox->readValidator(str);
                if(NULL!=validator)
                {
                    CountExecuteNode* countNode = new CountExecuteNode();
                    countNode->setValidator(validator);

                    previous->setNextNode(countNode);
                    node = countNode;
                    isFine = true;
                }
            }
                break;
            case Reroll:
            {
                Validator* validator = m_parsingToolbox->readValidator(str);
                if(NULL!=validator)
                {
                    RerollDiceNode* rerollNode = new RerollDiceNode();
                    rerollNode->setValidator(validator);
                    previous->setNextNode(rerollNode);
                    node = rerollNode;
                    isFine = true;


                }

            }
                break;
            case Explosing:
            {
                Validator* validator = m_parsingToolbox->readValidator(str);
                if(NULL!=validator)
                {
                    ExploseDiceNode* explosedNode = new ExploseDiceNode();
                    explosedNode->setValidator(validator);
                    previous->setNextNode(explosedNode);
                    node = explosedNode;
                    isFine = true;
                }
            }

            }
        }
    }




    return isFine;
}

bool DiceParser::readOperand(QString& str,ExecutionNode* & node)
{
    int myNumber=1;
    ExecutionNode* myNode=NULL;

    if(m_parsingToolbox->readNumber(str,myNumber))
    {
        NumberNode* myNumberNode = new NumberNode();
        myNumberNode->setNumber(myNumber);

        node = myNumberNode;
        return true;
    }
//    else if(readExpression(str,myNode))
//    {

//        if(readOperator(str,myNode))
//        {

//            if(readExpression(str,myNode))
//            {

//                m_parsingToolbox->readNumber(str,myNumber);
//                /// @todo implements this
//            }

//        }

//    }



        return false;
}
