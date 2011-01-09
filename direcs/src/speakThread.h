/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
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

#include <QtGlobal> // for Q_OS_* Makro!

//-------------------------------------------------------------------
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS at the moment)
#include "speak_lib.h"
#endif
#include <QThread>
//-------------------------------------------------------------------


/*!
\brief Speaks a text with espeak.

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
		void setLanguage(QString language);
		void setVoice(unsigned char gender,unsigned char age);
		void setRate(int value);


	public slots:
		/**
		Speaks a text with espeak. All HTML-Code in the parameter (text) is also removed internally).
		@param text is the text to speak.
		*/
		void speak(QString text);


	private:
		QString removeHTML(QString string);
		
		volatile bool stopped;
		bool saySomething; /// this is for the thread, which waits for something to say.
		QString textToSpeak;
		
		// Every thread sleeps some time, for having a bit more time fo the other threads!
		// Time in milliseconds
		static const unsigned long THREADSLEEPTIME = 600; // Default value: 600 ms
		
};

#endif
