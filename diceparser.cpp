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

   bool keepParsing = readExpression(str,newNode);


    if(keepParsing)
    {
        m_start->setNextNode(newNode);
//        if(NULL==m_start)
//        {
//            m_start = m_current;
//        }



        ExecutionNode* current = getLatestNode(m_start);



        keepParsing =!str.isEmpty();
        if(keepParsing)
        {
            keepParsing = readOperator(str,current);

            current = getLatestNode(current);
        }
        return true;
    }
    return false;

}

bool DiceParser::readExpression(QString& str,ExecutionNode* & node)
{
    int myNumber = 1;
    bool hasReadNumber=false;
    if(m_parsingToolbox->readOpenParentheses(str))
    {
        ExecutionNode* internalNode=NULL;
        if(readExpression(str,internalNode))
        {
                ParenthesesNode* parentheseNode  = new ParenthesesNode();
                parentheseNode->setInternelNode(internalNode);
                node = parentheseNode;
                //node->setNextNode(parentheseNode);
                if(m_parsingToolbox->readCloseParentheses(str))
                {
                    return true;
                }


        }
    }
    NumberNode* myNumberNode = NULL;
    if(NULL==node)
    {
        hasReadNumber= m_parsingToolbox->readNumber(str,myNumber);

        myNumberNode = new NumberNode();
        myNumberNode->setNumber(myNumber);
    }


    bool keepParsing = true;
    ExecutionNode* execNode=NULL;
    keepParsing = readDiceExpression(str,execNode);
    if(NULL!=myNumberNode)
    {
        myNumberNode->setNextNode(execNode);
        node = myNumberNode;
    }
    else
    {
        node = execNode;
    }
    if(NULL!=execNode)
    {
        execNode = getLatestNode(execNode);
        while(keepParsing)
        {
            keepParsing = readOperator(str,execNode);
            execNode = getLatestNode(execNode);
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
    while(NULL != next->getNextNode() )
    {
        next = next->getNextNode();
    }
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
    bool vectorDone=false;

    while(NULL!=myResult)
    {

        if((!scalarDone)&&(myResult->isScalar()))
        {
            stream << totalValue.arg(myResult->getScalar()) << endl; //.arg(m_command)
            scalarDone=true;
        }

        DiceResult* myDiceResult = dynamic_cast<DiceResult*>(myResult);
        if((NULL!=myDiceResult)&&(!vectorDone))
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


//qDebug() << "list:" << << " sum:" << << " command:" << command;
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
    if(m_parsingToolbox->readNumber(str,myNumber))
    {

    }
    if(readOperator(str,node))
    {

       m_parsingToolbox->readNumber(str,myNumber);
        /// @todo implements this

    }


}
