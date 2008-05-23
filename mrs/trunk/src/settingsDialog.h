#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>
#include <QDialog>

#include "ui_settingsDialog.h"


/**
Dialog for showing the current settings or to let them change / interact with the main window.
*/
class SettingsDialog : public QDialog
{
	Q_OBJECT
	
	public:
		SettingsDialog(QWidget* parent = 0, Qt::WFlags fl = 0);


	protected slots:
		/**
		Event handler for the "About Qt" button.
		*/
		//void on_btnAboutQt_clicked(bool checked);
		
	private:
		Ui::settingsDialog ui;
};

#endif
