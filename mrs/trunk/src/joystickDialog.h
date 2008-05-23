#ifndef JOYSTICKDIALOG_H
#define JOYSTICKDIALOG_H

#include <QtGui>
#include <QDialog>

#include "ui_joystickDialog.h"


/**
Dialog for showing information about the joystick movement and the pressed buttons.
*/
class JoystickDialog : public QDialog
{
	Q_OBJECT
	
	public:
		JoystickDialog(QWidget* parent = 0, Qt::WFlags fl = 0);


	protected slots:
		/**
		Event handler for the "About Qt" button.
		*/
		//void on_btnAboutQt_clicked(bool checked);
		
	private:
		Ui::joystickDialog ui;
};

#endif
