#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void readLine();
	
private slots:
	void sliderReleased();
	void buttonPressed(bool);
	void watchdogTimeout();
	
private:
	Ui::MainWindow *ui;
	bool connected;
	
	QSerialPort port;
	QByteArray txBuff;
	QByteArray rxBuff;
	
	QTimer watchdog;
};

#endif // MAINWINDOW_H
