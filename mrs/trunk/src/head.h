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

#ifndef HEAD_H
#define HEAD_H

//-------------------------------------------------------------------
#include "servo.h"
//-------------------------------------------------------------------

/**
\brief Gives access to the robots head.

The Head class gives access to the head of the robot. The head may turn right, up, down, smile, look left ... (to be defined/implemented).
*/
class Head : public QObject
{
    Q_OBJECT

	public:
		Head(Servo *s);
		~Head();
		

	public slots:
		/**
		@param direction where to look can be LEFT, RIGHT... or CURIOUS, NORMAL...
		*/ 
		void look(QString direction);
		
		
	private:
		void eye(unsigned char whichEye, QString direction);
		void eyebrow(unsigned char whichBrow, QString direction);
		Servo *servos;

		//! the possible eyes
		static const unsigned char LEFTEYE  = 1;
		static const unsigned char RIGHTEYE = 2;

		//! the possible eyebrows
		static const unsigned char LEFTBROW  = 1;
		static const unsigned char RIGHTBROW = 2;

		//! the possible servo data
		static const unsigned char SVSTART   = 0;
		static const unsigned char SVEND     = 1;
		static const unsigned char SVDEFAULT = 2;
		static const unsigned char SVCURRENT = 3;
		static const unsigned char SVMIN     = 4;
		static const unsigned char SVMAX     = 5;
		
		//! the servo numbers
		static const unsigned char SERVO1 = 0;
		static const unsigned char SERVO2 = 1;
		static const unsigned char SERVO3 = 2;
		static const unsigned char SERVO4 = 3;
		static const unsigned char SERVO5 = 4;
		static const unsigned char SERVO6 = 5;
};

#endif
