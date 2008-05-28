/*************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                  *
 *   mrs @ direcs.de                                                     *
 *                                                                       *
 *   This file is part of mrs.                                           *
 *                                                                       *
 *   mrs is free software: you can redistribute it and/or modify it      *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#include "speakThread.h"


SpeakThread::SpeakThread()
{
	stopped = false;
	speaking = false;
	
	
	//----------------------------------------------------------------------------
	// Initialize the speech engine festival
	//----------------------------------------------------------------------------
	#ifdef _TTY_POSIX_
	//      1 = we want the festival init files loaded
	// 500000 = default scheme heap size
	qDebug("festival initialized");
	festival_initialize(1, 500000);
	#endif
}


SpeakThread::~SpeakThread()
{
	stopped = false;
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
			qDebug("Speaking...");
			// FIXME: SIOD ERROR: the currently assigned stack limit has been exceded
			// file: ~/.festivalrc
			// (Parameter.set 'Audio_Command "aplay -q -c 1 -t raw -f s16 -r $SR $FILE")
			// (Parameter.set 'Audio_Method 'Audio_Command)
			// for speaking while audio is already in use!
			//
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
