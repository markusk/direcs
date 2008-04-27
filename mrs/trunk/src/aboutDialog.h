#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui>
#include <QDialog>

#include "ui_aboutDialog.h"


/**
Dialog for showing information about mrs.
*/
class AboutDialog : public QDialog
{
	Q_OBJECT
	
	public:
		AboutDialog(QWidget* parent = 0, Qt::WFlags fl = 0);


	protected slots:
		/**
		Event handler for the "About Qt" button.
		*/
		void on_btnAboutQt_clicked(bool checked);
		
	private:
		Ui::aboutDialog ui;
};

#endif
