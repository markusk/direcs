#ifndef INIFILE_H
#define INIFILE_H

//-------------------------------------------------------------------
#include <QtGui>
//-------------------------------------------------------------------

/**
\brief Handles the access to the global ini-file.

This class handles the access to the global ini-file where all settings are stored.
*/
class Inifile : public QObject
{
    Q_OBJECT

	public:
		Inifile();
		~Inifile();
		
		/**
		Checks the current path from which the program started.
		@return The path as a string.
		*/
		QString checkPath();
		
		/**
		Checks if the needed ini-file exists in the current path.
		@return True, when the file was found.
		*/
		bool checkFiles();
		
		/**
		Stores a settings in an ini-file. The settings are stored in the typical ini-file-format.\n
		Example ini-file:\n
		\n
		[Config]\n
		motor1Speed=23
		@param group is the group area in the file (e.g. \e Config).
		@param name is the name of the parameter (e.g. \e motor1speed).
		@param value is the value of the parameter (e.g. \e 23).
		*/
		void writeSetting(QString group, QString name, int value);
		
		/**
		Reads an integer setting from an ini-file. The settings are read in the typical ini-file-format.\n
		Example ini-file:\n
		\n
		[Config]\n
		motor1Speed=23
		@param group is the group area in the file (e.g. \e Config).
		@param name is the object to read (e.g. \e motor1speed).
		@return The setting as an integer value.
		*/
		int readSetting(QString group, QString name);
		
		/**
		Reads a string from an ini-file. The settings are read in the typical ini-file-format.\n
		Example ini-file:\n
		\n
		[Config]\n
		serialPortMicrocontroller=/dev/ttyUSB1
		@param group is the group area in the file (e.g. \e Config).
		@param name is the object to read (e.g. \e serialPortMicrocontroller).
		@return The setting as a string.
		*/
		QString readString(QString group, QString name);
		
		/**
		Forces the ini.file to be written \e immediately. For example at program end.
		*/
		void sync(void);
		
		/**
		@return The current ini-file.
		*/
		QString getInifileName();

	//private slots:
		//void commandClock();

	private:
		//Gui *gui1;
		QSettings* settings;
		QString programPath;
		QString mainIniFilename;
		//QTimer* commandTimer;
		//QString iniSection;
		//QString lastCommand;
		//int commandCounter;
};

#endif
