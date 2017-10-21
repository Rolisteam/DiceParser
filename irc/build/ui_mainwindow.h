/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <qbotircdiceparser.h>

QT_BEGIN_NAMESPACE

class Ui_BotIrcDiceParser
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *m_connectButton;
    QPushButton *m_joinButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_disconnectButton;
    QTextEdit *m_output;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QBotIrcDiceParser *BotIrcDiceParser)
    {
        if (BotIrcDiceParser->objectName().isEmpty())
            BotIrcDiceParser->setObjectName(QStringLiteral("BotIrcDiceParser"));
        BotIrcDiceParser->resize(604, 458);
        centralwidget = new QWidget(BotIrcDiceParser);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        m_connectButton = new QPushButton(centralwidget);
        m_connectButton->setObjectName(QStringLiteral("m_connectButton"));

        horizontalLayout->addWidget(m_connectButton);

        m_joinButton = new QPushButton(centralwidget);
        m_joinButton->setObjectName(QStringLiteral("m_joinButton"));

        horizontalLayout->addWidget(m_joinButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_disconnectButton = new QPushButton(centralwidget);
        m_disconnectButton->setObjectName(QStringLiteral("m_disconnectButton"));

        horizontalLayout->addWidget(m_disconnectButton);


        verticalLayout->addLayout(horizontalLayout);

        m_output = new QTextEdit(centralwidget);
        m_output->setObjectName(QStringLiteral("m_output"));

        verticalLayout->addWidget(m_output);

        menubar = new QMenuBar(BotIrcDiceParser);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 604, 25));
        statusbar = new QStatusBar(BotIrcDiceParser);
        statusbar->setObjectName(QStringLiteral("statusbar"));

        retranslateUi(BotIrcDiceParser);

        QMetaObject::connectSlotsByName(BotIrcDiceParser);
    } // setupUi

    void retranslateUi(QBotIrcDiceParser *BotIrcDiceParser)
    {
        BotIrcDiceParser->setWindowTitle(QApplication::translate("BotIrcDiceParser", "BotIrcDiceParser", 0));
        m_connectButton->setText(QApplication::translate("BotIrcDiceParser", "Connect", 0));
        m_joinButton->setText(QApplication::translate("BotIrcDiceParser", "join", 0));
        m_disconnectButton->setText(QApplication::translate("BotIrcDiceParser", "Disconnect", 0));
    } // retranslateUi

};

namespace Ui {
    class BotIrcDiceParser: public Ui_BotIrcDiceParser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
