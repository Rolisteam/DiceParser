#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DiceParser;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

protected slots:
	void rollDiceCommand();
private:
	Ui::MainWindow *ui;
	DiceParser* m_dieParser;
};

#endif // MAINWINDOW_H
