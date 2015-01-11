#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <qextserialport.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void init_port(); // initialisation du port

public slots:
	void transmitCmd(int value); // fonction d'envoi

private:
	Ui::MainWindow *ui;
	QextSerialPort *port;
};

#endif // MAINWINDOW_H
