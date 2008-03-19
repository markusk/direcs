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
		@param direction where to look can be LEFT, RIGHT...
		*/ 
		void look(QString direction);
		
		
	private:
		void eye(unsigned char whichEye, QString direction);
		Servo *servos;

		//! the possible eyes
		static const unsigned char LEFTEYE  = 1;
		static const unsigned char RIGHTEYE = 2;

		//! the possible servo data
		static const unsigned char SVSTART   = 0;
		static const unsigned char SVEND     = 1;
		static const unsigned char SVDEFAULT = 2;
		static const unsigned char SVCURRENT = 3;
		
		//! the servo numbers
		static const unsigned char SERVO1 = 0;
		static const unsigned char SERVO2 = 1;
		static const unsigned char SERVO3 = 2;
		static const unsigned char SERVO4 = 3;
		static const unsigned char SERVO5 = 4;
		static const unsigned char SERVO6 = 5;
};

#endif
