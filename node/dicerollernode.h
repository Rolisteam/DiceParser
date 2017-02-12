#ifndef DICEROLLERNODE_H
#define DICEROLLERNODE_H

#include <Qt>

#include "executionnode.h"
#include "result/diceresult.h"

/**
 * @brief The DiceRollerNode class rolls dice of one kind.
 */
class DiceRollerNode : public ExecutionNode
{
public:
	/**
	 * @brief DiceRollerNode builds an instance
	 * @param faces, number of faces of dices
	 * @param offset, first value of dice.
	 */
    DiceRollerNode(quint64 max, qint64 min = 1);

	/**
	 * @brief run - starts to roll dice.
	 */
    virtual void run(ExecutionNode*);
	/**
	 * @brief getFaces accessor
	 * @return the face count
	 */
    quint64 getFaces() const;

	/**
	  * @brief toString
	  * @param wl
	  * @return use to generate dot tree;
	  */
	 virtual QString toString(bool wl)const;
	/**
	  * @brief getPriority
	  * @return priority for dice roll: 4 (higher)
	  */
     virtual qint64 getPriority() const;

    virtual ExecutionNode* getCopy() const;

//private members
private:
    quint64 m_diceCount;
    qint64 m_max; /// faces
	DiceResult* m_diceResult;
    qint64 m_min;
};

#endif // DICEROLLERNODE_H
