//
// C++ Interface: speakThread
//
// Description: 
//
//
// Author: Markus Knapp <webmaster@direcs.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SPEAKTHREAD_H
#define SPEAKTHREAD_H

//-------------------------------------------------------------------
#include <festival.h>
#include "speakThread.h"
#include <QThread>
//-------------------------------------------------------------------


/*!
\brief Speaks a text with festival.

Also removes HTML-Tags from the text to speak.
*/
class SpeakThread : public QThread
{
    Q_OBJECT

	public:
		SpeakThread();
		~SpeakThread();
		void stop();
		virtual void run();


	public slots:
		void speak(QString text);


	private:
		//mutable QMutex mutex; // make this class thread-safe
		QString removeHTML(QString string);
		volatile bool stopped;
		bool speaking;
		#ifdef _TTY_POSIX_
		EST_String textForFestival;
		#endif
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 600; // Default: 600 ms
		
		static const unsigned char FLAG = 1;
};

#endif
