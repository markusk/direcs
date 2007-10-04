//
// C++ Interface: laser
//
// Description: 
//
//
// Author: Markus Knapp <webmaster@direcs.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LASER_H
#define LASER_H

#include <QtGlobal> // for qDebug

extern void startlaser (void);
extern void shutdown_laserview(void);
extern float getLaserDistance(int angle);
extern int getLaserNumReadings(void);



#endif
