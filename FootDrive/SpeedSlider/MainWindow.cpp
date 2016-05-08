#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	txBuff.append("SS");
	txBuff.append((char)0);
	txBuff.append((char)0);
	txBuff.append((char)0);
	txBuff.append((char)0);
	txBuff.append((char)0);
	txBuff.append((char)13);
	
	connected = false;
	
	connect(ui->horizontalSlider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
	
	connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(buttonPressed(bool)));
	
	watchdog.start(50);
	connect(&watchdog, SIGNAL(timeout()), this, SLOT(watchdogTimeout()));
}

MainWindow::~MainWindow()
{
	if (connected) buttonPressed(false);
	
	delete ui;
}

void MainWindow::readLine()
{	
	rxBuff.append(port.read(port.bytesAvailable()));
	qDebug() << rxBuff;
	rxBuff.clear();
}

void MainWindow::sliderReleased()
{
	//ui->horizontalSlider->setValue(0);
}

void MainWindow::buttonPressed(bool pressed)
{
	if (pressed)
	{
		port.setBaudRate(115200);
		port.setPortName("COM3");
		port.open(QIODevice::ReadWrite);
	}
	else
	{
		port.close();
	}
	
	connected = port.isOpen();
	ui->pushButton->setChecked(connected);
	
	if (connected) ui->pushButton->setText("Disconnect");
	else ui->pushButton->setText("Connect");
}

void MainWindow::watchdogTimeout()
{
	int value = ui->horizontalSlider->value();
	QString speed = QString("%1").arg(abs(value), 4, 10, QChar('0'));
	
	char* txPtr = txBuff.data();
	txPtr += 2;
	
	if (value > 0) *txPtr = '+';
	else *txPtr = '-';
	
	for (unsigned int i = 0; i < 4; ++i)
	{
		txPtr++;
		*txPtr = speed.at(i).toLatin1();
	}
	
	qDebug() << txBuff;
	
	if (connected)
	{
		port.write(txBuff);
		readLine();
	}
}
