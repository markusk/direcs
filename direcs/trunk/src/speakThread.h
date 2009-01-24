/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/

#ifndef SPEAKTHREAD_H
#define SPEAKTHREAD_H

//-------------------------------------------------------------------
#include <festival.h>
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
