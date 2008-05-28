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

#ifndef CARMEN_LASER_MAIN_H
#define CARMEN_LASER_MAIN_H

// from laser.h:
#define     CARMEN_FRONT_LASER_NUM       0
#define     CARMEN_REAR_LASER_NUM        1
#define     CARMEN_LASER3_NUM            2
#define     CARMEN_LASER4_NUM            3
#define     CARMEN_LASER5_NUM            4


#include "sick.h"
#include "laser_messages.h"
#include "param_interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <QtGlobal>
#include <QString>
#include <QDebug>


/*!
For the mrs laserThread
*/
static const short int LASER1 = 1;
static const short int LASER2 = 2;
static const short int LASER3 = 4;
static const short int LASER4 = 8;
static const short int LASER5 = 16;


//
// declarations
//
int carmen_laser_start(short int laserScanner);
void carmen_laser_shutdown(int x);
int carmen_laser_run(void);
void shutdown_laser(int x);
int getLaserNumReadings(int laser);
float getLaserDistance(int laser, int angle);
void setDevicePort(short int laser, QString serialPort);


// FROM global.h:
extern inline double carmen_radians_to_degrees(double theta)
{
  return (theta * 180.0 / M_PI);
}

extern inline double carmen_degrees_to_radians(double theta)
{
  return (theta * M_PI / 180.0);
}


#endif
