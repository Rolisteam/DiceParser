#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "diceparser.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->m_rollButton,SIGNAL(pressed()),this,SLOT(rollDiceCommand()));
	 m_dieParser = new DiceParser();
}

MainWindow::~MainWindow()
{
	delete ui;
}
void MainWindow::rollDiceCommand()
{
	QString cmd = ui->m_cmdEdit->text();

	if(m_dieParser->parseLine(cmd))
	{
		m_dieParser->Start();
		m_dieParser->displayResult();
	}

}
