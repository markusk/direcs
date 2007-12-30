//
// C++ Implementation: speakThread
//
// Description: 
//
//
// Author: Markus Knapp <webmaster@direcs.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "speakThread.h"


SpeakThread::SpeakThread()
{
	stopped = false;
	speaking = false;
	
	//QMutexLocker locker(&mutex); // make this class thread-safe
	
	
	//----------------------------------------------------------------------------
	// Initialize the speech engine festival
	//----------------------------------------------------------------------------
	#ifdef _TTY_POSIX_
	//      1 = we want the festival init files loaded
	// 500000 = default scheme heap size
	//qDebug("hello");
	// FIXME: check, if /dev/dsp is openable (e.g. when in use (amarok))
	festival_initialize(1, 500000);
	#endif
}


SpeakThread::~SpeakThread()
{
	stopped = false;
	
	//QMutexLocker locker(&mutex); // make this class thread-safe
}


void SpeakThread::stop()
{
	stopped = true;
}


void SpeakThread::run()
{
	//
	//  start "threading"...
	//
	while (!stopped)
	{
		// let the thread sleep some time
		// for having more time for the other threads
		msleep(THREADSLEEPTIME);
		
		if (speaking==true)
		{
			#ifdef _TTY_POSIX_
			// say the text
			// FIXME: check, if /dev/dsp is openable (e.g. when in use (amarok))
			festival_say_text(textForFestival);
			#endif
			speaking=false;
		}
		
	}
	stopped = false;
}


void SpeakThread::speak(QString text)
{
	#ifdef _TTY_POSIX_
	// remove HTML tags from string
	text = removeHTML(text);
	
	// convert QString to EST_String (defined in EST_String.h)
	textForFestival = EST_String(text.toAscii());
	
	speaking = true;
	#endif
	
}


QString SpeakThread::removeHTML(QString string)
{
	int start= -1;
	
	
	//------------------------------
	// remove HTML tags from string
	//------------------------------
	do
	{
		// search for the first HTML "<"
		start = string.indexOf("<");

		if (start != 1)
		{
			string.remove(start, string.indexOf(">")+1 - start);
		}
	} while (string.contains(">"));
	// till the last HTML ">" is found
	
	return string;
}
