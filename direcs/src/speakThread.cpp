/*************************************************************************
 *   Copyright (C) Markus Knapp                                          *
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
	mPhase = 0;

#ifdef Q_OS_LINUX
	// Synchronous playback
	// 0 ms length sound buffer for SynthCallBack
	// default location for espeak-data directory
	// dont allow espeakEVENT_PHONEME events
	espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL,0);
	// set speech rate down to 150 words per minute
	setRate(150);
#else

//    serialPort = new DirecsSerialQext();

    // creating the QtSpeech object
    voice = new QtSpeech();
//    voice.id = "macosx:8";
//    voice.name = "Zarvox";
#endif
}


SpeakThread::~SpeakThread()
{
#ifdef Q_OS_LINUX
	espeak_Terminate();
#endif
	saySomething = false;
	stopped = false;
}


void SpeakThread::stop()
{
#ifdef Q_OS_LINUX
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

#ifdef Q_OS_LINUX
		if (saySomething == true)
		{
			saySomething = false;

			// remove HTML tags from string (needed for reading messages from the GUI log)
			textToSpeak = removeHTML(textToSpeak);

			// speak!
			espeak_Synth( textToSpeak.toAscii(), textToSpeak.length()+1, 0, POS_CHARACTER, 0, espeakCHARS_AUTO, NULL, NULL );
			espeak_Synchronize();

			// let other Slots know that we completed the sentence.
			emit speechCompleted(mPhase);
		}
#endif

	}
	stopped = false;
}


void SpeakThread::speak(QString text, int phase)
{
#ifdef Q_OS_LINUX
	// store the text in the class member
	textToSpeak = text;

	// store the phase locally
	mPhase = phase;

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
	Q_UNUSED(phase);
#endif
}


#ifdef Q_OS_LINUX
void SpeakThread::setLanguage(QString language)
{
	espeak_SetVoiceByName(language.toAscii());
}
#endif


#ifdef Q_OS_LINUX
void SpeakThread::setRate(int value)
{
	espeak_SetParameter(espeakRATE, value, 0);
}
#endif


#ifdef Q_OS_LINUX
void SpeakThread::setVoice(unsigned char gender,unsigned char age)
{
    espeak_VOICE *voice_spec=espeak_GetCurrentVoice();
    voice_spec->gender=gender;
    voice_spec->age = age;
    espeak_SetVoiceByProperties(voice_spec);
}
#else
void setVoice(QString voicename)
{
    // Deutsch
    voice.id = "macosx:274733789";
}
#endif


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
