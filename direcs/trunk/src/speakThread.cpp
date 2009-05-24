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

#include "speakThread.h"


SpeakThread::SpeakThread()
{
	stopped = false;
	speaking = false;
	saySomething = false;
	
#ifdef _TTY_POSIX_
	espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL,0); 
	// set speech rate down to 150 words per minute
	setRate(150);
#endif
}


SpeakThread::~SpeakThread()
{
	saySomething = false;
	stopped = false;
#ifdef _TTY_POSIX_
	espeak_Terminate();
#endif
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
		
#ifdef _TTY_POSIX_
		if (saySomething == true)
		{
			saySomething = false;
			
			// remove HTML tags from string
			textToSpeak = removeHTML(textToSpeak);
			
			// speak!
			espeak_Synth( textToSpeak.toAscii(), textToSpeak.length()+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL ); 
			espeak_Synchronize(); 
		}
#endif
		
	}
	stopped = false;
}


void SpeakThread::speak(QString text)
{
#ifdef _TTY_POSIX_
	// store the text in the class member
	textToSpeak = text;
	// enbale the run method to speak :-)
	saySomething = true;
#endif
}


void SpeakThread::lang(const char *lang)
{
#ifdef _TTY_POSIX_
	espeak_SetVoiceByName(lang);
#endif
}


void SpeakThread::setRate(int value)
{
#ifdef _TTY_POSIX_
	espeak_SetParameter(espeakRATE, value, 0);
#endif
}


void SpeakThread::setVoices(unsigned char gender,unsigned char age)
{
#ifdef _TTY_POSIX_
	espeak_VOICE *voice_spec=espeak_GetCurrentVoice();
	voice_spec->gender=gender;
	voice_spec->age = age;
	espeak_SetVoiceByProperties(voice_spec);
#endif
}


QString SpeakThread::removeHTML(QString string)
{
	int start= -1;
	
	
	do
	{
		// search for the first HTML "<"
		start = string.indexOf("<");

		if (start != 1)
		{
			string.remove(start, string.indexOf(">")+1 - start);
		}
	} while (string.contains(">"));
	// to the last HTML ">" found
	
	return string;
}
