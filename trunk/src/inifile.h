#ifndef INIFILE_H
#define INIFILE_H

//-------------------------------------------------------------------
#include <QtGui>
//-------------------------------------------------------------------


class Inifile : public QObject
{
    Q_OBJECT

	public:
		Inifile();
		~Inifile();
		QString checkPath();
		bool checkFiles();
		void writeSetting(QString group, QString name, int value);
		int readSetting(QString group, QString value);
		QString readString(QString group, QString name);
		void sync(void);
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
