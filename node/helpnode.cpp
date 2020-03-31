/***************************************************************************
 *   Copyright (C) 2015 by Renaud Guezennec                                *
 *   http://www.rolisteam.org/contact                   *
 *                                                                         *
 *   rolisteam is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "helpnode.h"

HelpNode::HelpNode() : m_path("https://github.com/Rolisteam/DiceParser/blob/master/HelpMe.md")
{
    m_result= new StringResult();
}
void HelpNode::run(ExecutionNode* previous)
{
    m_previousNode= previous;
    StringResult* txtResult= dynamic_cast<StringResult*>(m_result);
    txtResult->setHighLight(false);

    if((nullptr == previous) && (txtResult != nullptr))
    {
        txtResult->addText(QObject::tr("Rolisteam Dice Parser:\n"
                                       "\n"
                                       "Example (with ! as prefix):\n"
                                       "!2d6\n"
                                       "!1d20\n"
                                       "!6d10e10k3 (L5R)\n"
                                       "\n"
                                       "Full documentation at: %1")
                               .arg(m_path));
        /*txtResult->setText(QObject::tr("Rolisteam Dice Parser:\n"
                                       "\n"
                                       "Example (with ! as prefix):\n"
                                       "!2d6\n"
                                       "!1d20\n"
                                       "\n"
                                       "Operator list:\n"
                                       "\n"
                                        "k : Keep             : 2d10k1       => roll two 10-sided dice and keep the
           higher one (kl1 for smaller one)\n" "K : Keep And Explode : 2d10K1       => Equivalent of 2d10e10k1\n" "s :
           Sort             : 8d10         => roll eight 10-sided dice and sort the result list\n" "c : Count :
           8d10c[>7]    => roll eight 10-sided dice and count how many dice are higher than 7\n" "r : Reroll           :
           8d6r1        => roll eight 6-sided dice and reroll dice once if its result is 1. (result of the reroll can be
           1)\n" "e : Explode          : 8d10e10      => roll eight 10-sided dice and while dice makes a 10 it is
           reroll. The result is added to those dice.\n" "a : Reroll and add   : 8d6a1        => roll eight 6-sided dice
           and reroll dice once and the result is added at 1\n" "m : Merge            : 1d20;1d10mk1 => roll one 20-side
           die and one 10-sided die and keep the higher die\n" "i : if               : 1d100i[=100]{\"jackpot\"} => Roll
           one 100-side die and display \"jackpot\" if the die result is 100.\n" "f : filter           : 4d10f[!=4] =>
           roll four 10-sided dice and ignore all dice with 4 as result"
                                        "; : Next instruction : 1d20;2d10;3d8 => roll one 20-sided die, two 10-sided
           dice and three 8-sided dice \n" "g : Group            : 8d10g10       => count how many group of 10 it is
           possible to do (according to rule of 7th sea).\n"
                                        "# : Comment          : 1d2 #flip coin=> display flip coin as comment of 1 or 2
           result.\n"
                                        "\n"
                                        "Validator:\n"
                                        "\n"
                                        "Supported conditions: >,<,=,>=,<=,!=\n"
                                        "Supported operators: % (modulo), &,^,| \n"
                                        "\n"
                                        "	Examples:\n"
                                        "\n"
                                        "c[>7 & %2=0] : count how many dice are higher than 7 and even\n"
                                        "c[>7 | %2=0] : count how many dice are higher than 7 or even\n"
                                        "c[>7 ^ %2=0] : count how many dice are higher than 7 or even but not higher
           than 7 and even\n"
                                        "\n"
                                        "List:\n"
                                        "\n"
                                        "1L[green,blue] => pick value from the list (green or blue)\n"
                                        "2L[green,blue] => pick two values from the list (green,green | green,blue |
           blue,green | blue,blue)\n" "2Lu[green,blue] => pick two unique values from the list (green,blue |
           blue,green)\n"
                                        "\n"
                                        "Arithmetic\n"
                                        "\n"
                                        "8+8+8      => 24\n"
                                        "24-4       => 20\n"
                                        "(3+4)*2    => 14\n"
                                        "7/2        => 3.5\n"
                                        "2^4        => 16\n"
                                        "1d6+6      => roll one 6-sided die and add 6 to its result\n"
                                        "(2d4+2)d10 => roll two 4-sided dice, add 2 to the result[2;8] then roll from
           four to ten 10-sided dice\n"
                                        "\n"
                                        "Full documentation at: %1").arg(m_path));*/
        m_result->setPrevious(nullptr);
    }
    else if(nullptr != previous)
    {
        txtResult->addText(previous->getHelp());
        m_result->setPrevious(previous->getResult());
    }
    txtResult->finished();

    if(nullptr != m_nextNode)
    {
        m_nextNode->run(this);
    }
}
QString HelpNode::toString(bool wl) const
{
    if(wl)
    {
        return QString("%1 [label=\"Rolisteam Dice Parser:\nFull documentation at: %2\"]").arg(m_id, m_path);
    }
    else
    {
        return m_id;
    }
}

qint64 HelpNode::getPriority() const
{
    return 0;
}
void HelpNode::setHelpPath(QString path)
{
    m_path= path;
}

ExecutionNode* HelpNode::getCopy() const
{
    HelpNode* node= new HelpNode();
    if(nullptr != m_nextNode)
    {
        node->setNextNode(m_nextNode->getCopy());
    }
    return node;
}
