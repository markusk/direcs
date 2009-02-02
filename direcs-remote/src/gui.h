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
#include "direcs-remote.h"
#include <QtGui>
#include <QWebView>
/*
#include <Phonon/Global>
#include <Phonon/MediaObject>
#include <Phonon/Path>
#include <Phonon/AudioOutput>
//-------------------------------------------------------------------
*/

// forward declarations because of circular includes!
class DirecsRemote;

class Gui : public QMainWindow
{
	Q_OBJECT

	public:
		Gui(DirecsRemote *m, QMainWindow *parent = 0);
		~Gui();
		int getPort();
		void appendLog(QString text, bool sayIt=false);
	
	
	public slots:
		void showMotorCurrent(int motor, int value);
	
	
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
		void on_btnShutdown_clicked();
		void on_spinBoxSpeed1_valueChanged(int value);
		void on_spinBoxSpeed2_valueChanged(int value);
		void on_btnGo_clicked();


	private:
		Ui::Gui ui;
		DirecsRemote *direcsremote1;
		QWebView *view;
		//MediaObject *media;
		//VideoWidget *vwidget;
		bool robotDrives;
		int speedMotor1;
		int speedMotor2;
};

#endif
