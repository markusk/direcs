/*  QtSpeech -- a small cross-platform library to use TTS
	Copyright (C) 2010-2011 LynxLine.

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 3 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General
	Public License along with this library; if not, write to the
	Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
	Boston, MA 02110-1301 USA */

#include <QtCore>
#include <QtSpeech>

int main(int argc, char ** argv)
{
	QCoreApplication a(argc, argv);
	QString text; // the text to speak

	/*
	foreach(QtSpeech::VoiceName v, QtSpeech::voices())
		qDebug() << "id:" << v.id << "name:" << v.name;
	*/

	//--------------------------
	// use system default voice
	//--------------------------
	// QtSpeech speech;
	// text = "Das ist ja wohl endlich mal eine coole Sprachausgabe, oder was? Heisser Scheiss, Alter!";

	//--------------------
	// use specific voice
	//--------------------
	/*
	id: "macosx:1" name: "Fred"
	id: "macosx:2" name: "Kathy"
	id: "macosx:3" name: "Princess"
	id: "macosx:4" name: "Junior"
	id: "macosx:5" name: "Ralph"
	id: "macosx:6" name: "Whisper"
	id: "macosx:8" name: "Zarvox"
	id: "macosx:9" name: "Trinoids"
	id: "macosx:16" name: "Boing"
	id: "macosx:26" name: "Bells"
	id: "macosx:30" name: "Hysterical"
	id: "macosx:31" name: "Pipe Organ"
	id: "macosx:35" name: "Cellos"
	id: "macosx:36" name: "Bad News"
	id: "macosx:38" name: "Deranged"
	id: "macosx:39" name: "Good News"
	id: "macosx:40" name: "Bahh"
	id: "macosx:41" name: "Albert"
	id: "macosx:50" name: "Bubbles"
	id: "macosx:100" name: "Bruce"
	id: "macosx:200" name: "Vicki"
	id: "macosx:200" name: "Victoria"
	id: "macosx:201" name: "Alex"
	id: "macosx:300" name: "Agnes"
	id: "macosx:886" name: "Steffi"
	id: "macosx:268828893" name: "Yannick"
	*/
	//
	QtSpeech::VoiceName voice;
	voice.id = "macosx:8";
	voice.name = "Markus";
	QtSpeech speech1(voice);
	text = "Oh my god! I am a fucking lonesome robot but I do not have a robot girl. I am so depressed.";

	qDebug() << "About to say synchrounously" << text << "using voice:" << speech1.name().name;
	speech1.say(text);
	//return 0;


	//
	// Deutsch
	voice.id = "macosx:268828893";
	voice.name = "Yannick";
	QtSpeech speech2(voice);
	text = "Das ist ja wohl endlich mal eine coole Sprachausgabe, oder was? Robotik-Labor, der Pott-Kast rund um Robotik-Themen. www.robotiklabor.de. Heisser Scheiss, was?";

	qDebug() << "About to say asynchrounously" << text << "using voice:" << speech2.name().name;
	speech2.tell(text, &a, SLOT(quit()));

	return a.exec();
}
