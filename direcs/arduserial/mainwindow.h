#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <qextserialport.h> /// This is for serial port communication
#include <qextserialenumerator.h> /// This is for getting a list of serial ports

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	/**
	 * @brief initSerialPort Initialisation of the serial port
	 * @return true on success
	 */
	bool initSerialPort();


public slots:
	/**
	 * @brief transmitCmd sends a value to the Arduino.
	 * @param value which will be sent to the Arduino
	 */
	void sendValue(int value);


private slots:
	/**
	 * @brief arduinoInit Sends data to the Arduino on programm startup.
	 * Please note, that this is a Slot, called by the @sa timerEvent
	 */
	void arduinoInit();

	/**
	 * @brief onReadyRead is automatically called, when data on the serial port are available from the Arduino.
	 */
	void onReadyRead();

	/**
	 * @brief onPortAdded is called, if a USB device is added.
	 */
	void onPortAdded(QextPortInfo newPortInfo);

	/**
	 * @brief onPortRemoved is called, if a USB device is removed.
	 */
	void onPortRemoved(QextPortInfo newPortInfo);

	/**
	 * @brief showPorts shows the ports found by event.
	 * @param portInfos contains the found port to be shown
	 */
	void showPorts(QextPortInfo portInfos);



private:
	Ui::MainWindow *ui; /// The main window (GUI)
	QextSerialPort *port; /// The serial port
	QextSerialEnumerator *enumerator; /// This is for getting a list of serial ports (filenames like /dev/ttyUSB0)
	QString serialPortName; /// for the (file)name of the serial port, like /dev/ttyUSB0 or COM1
	int n;
};

#endif // MAINWINDOW_H
