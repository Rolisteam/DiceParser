#ifndef DICEROLLERNODE_H
#define DICEROLLERNODE_H

#include <Qt>
#include <QRunnable>
#include <QMutex>

#include "executionnode.h"
#include "result/diceresult.h"
class DiceRoller : public QRunnable
{
public:
    DiceRoller(QMutex* mutex,DiceResult* diceResult,int faces,int count);
    virtual void run ();
private:
    QMutex* m_mutex;
    DiceResult* m_sharedDiceResult;
    int m_faces;
    quint64 m_diceCount;
};

/**
 * @brief The DiceRollerNode class
 */
class DiceRollerNode : public ExecutionNode
{
public:
    DiceRollerNode(quint64 faces);

    virtual void run(ExecutionNode*);
    quint64 getFaces();

     virtual QString toString()const;
     virtual qint64 getPriority() const;
//private members
private:
    quint64 m_diceCount;
    quint64 m_faces; /// faces
    DiceResult* m_myDiceResult;
    QMutex* m_mutex;
};

#endif // DICEROLLERNODE_H
