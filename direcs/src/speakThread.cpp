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
	saySomething = false;
	
#ifdef Q_OS_LINUX // supported only under linux (no MAC OS, Windoze at the moment)
	// Synchronous playback
	// 0 ms length sound buffer for SynthCallBack
	// default location for espeak-data directory
	// dont allow espeakEVENT_PHONEME events
	espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL,0); 
	// set speech rate down to 150 words per minute
	setRate(150);
#endif
}


SpeakThread::~SpeakThread()
{
#ifdef Q_OS_LINUX // supported only under linux (no MAC OS, Windoze at the moment)
	espeak_Terminate();
#endif
	saySomething = false;
	stopped = false;
}


void SpeakThread::stop()
{
#ifdef Q_OS_LINUX // currently supported only under linux (no MAC OS and Windoze at the moment)
	// check if already speaking
	if (espeak_IsPlaying() == 1)
	{
		// shut up
		espeak_Cancel();
	}
#endif
	
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
		
#ifdef Q_OS_LINUX // supported only under linux (no MAC OS, Windoze at the moment)
		if (saySomething == true)
		{
			saySomething = false;
			
			// remove HTML tags from string (needed for reading messages from the GUI log)
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
#ifdef Q_OS_LINUX // supported only under linux (no MAC OS, Windoze at the moment)
	// store the text in the class member
	textToSpeak = text;
	// enbale the run method to speak :-)
	saySomething = true;
	
	// check if already speaking
	if (espeak_IsPlaying() == 1)
	{
		// shut up
		espeak_Cancel();
	}
#else
	Q_UNUSED(text);
#endif
}


void SpeakThread::setLanguage(QString language)
{
#ifdef Q_OS_LINUX // joystick support only under linux (no MAC OS, Windoze at the moment)
	espeak_SetVoiceByName(language.toAscii());
#else
	Q_UNUSED(language);
#endif
}


void SpeakThread::setRate(int value)
{
#ifdef Q_OS_LINUX // joystick support only under linux (no MAC OS, Windoze at the moment)
	espeak_SetParameter(espeakRATE, value, 0);
#else
	Q_UNUSED(value);
#endif
}


void SpeakThread::setVoice(unsigned char gender,unsigned char age)
{
#ifdef Q_OS_LINUX // supported only under linux (no MAC OS, Windoze at the moment)
	espeak_VOICE *voice_spec=espeak_GetCurrentVoice();
	voice_spec->gender=gender;
	voice_spec->age = age;
	espeak_SetVoiceByProperties(voice_spec);
#else
	Q_UNUSED(gender);
	Q_UNUSED(age);
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
