#ifndef GUI_H
#define GUI_H

//-------------------------------------------------------------------
// some driving directions for the robot
//
#define	FORWARD				1
#define	BACKWARD			2
#define	LEFT				3
#define	RIGHT				4
#define START				5
#define STOP				6
//-------------------------------------------------------------------

//-------------------------------------------------------------------
#include "ui_gui.h"
#include "mrs-remote.h"
#include <QtGui>
#include <QWebView>
//-------------------------------------------------------------------


// forward declarations because of circular includes!
class MrsRemote;

class Gui : public QMainWindow
{
	Q_OBJECT

	public:
		Gui(MrsRemote *m, QMainWindow *parent = 0);
		~Gui();
		int getPort();
		void appendLog(QString text, bool sayIt=false);
	
	
	protected:
		void closeEvent();
	
	signals:
		void commandIssued(QString command);
	
	private slots:
		void on_btnClear_clicked();
		void on_btnExit_clicked();
		void on_btnTest_clicked();
		void on_lblLink_linkActivated(QString link);
		void on_lblLink_linkHovered(QString link);
		void on_btnForward_clicked();
		void on_btnBackward_clicked();
		void on_btnLeft_clicked();
		void on_btnRight_clicked();
		void on_btnStartStop_clicked();
		void on_spinBoxSpeed1_valueChanged(int value);
		void on_spinBoxSpeed2_valueChanged(int value);


	private:
		Ui::Gui ui;
		MrsRemote *mrsremote1;
		QWebView *view;
		bool robotDrives;
		int speedMotor1;
		int speedMotor2;
};

#endif
