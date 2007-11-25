/*********************************************************
 *
 * This source code is part of the Carnegie Mellon Robot
 * Navigation Toolkit (CARMEN)
 *
 * CARMEN Copyright (c) 2002 Michael Montemerlo, Nicholas
 * Roy, Sebastian Thrun, Dirk Haehnel, Cyrill Stachniss,
 * and Jared Glover
 *
 * CARMEN is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; 
 * either version 2 of the License, or (at your option)
 * any later version.
 *
 * CARMEN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU General 
 * Public License along with CARMEN; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, 
 * Suite 330, Boston, MA  02111-1307 USA
 *
 ********************************************************/

#ifndef CARMEN_LASER_MAIN_H
#define CARMEN_LASER_MAIN_H
/*
Markus Original
#ifdef __cplusplus
extern "C" {
#endif
*/

//----------------
//Markus:
// from laser.h
#define     CARMEN_FRONT_LASER_NUM       0
#define     CARMEN_REAR_LASER_NUM        1
#define     CARMEN_LASER3_NUM            2
#define     CARMEN_LASER4_NUM            3
#define     CARMEN_LASER5_NUM            4

#include "param_interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <QtGlobal>
//----------------

// Markus:
/*!
For the mrs laserThread
*/
static const short int LASER1 = 1;
static const short int LASER2 = 2;
static const short int LASER3 = 4;
static const short int LASER4 = 8;
static const short int LASER5 = 16;






// Markus Original: int carmen_laser_start(int argc, char **argv);
int carmen_laser_start(void);

void carmen_laser_shutdown(int x);

int carmen_laser_run(void);

void shutdown_laser(int x);

//---------------
// FROM global.h
extern inline double carmen_radians_to_degrees(double theta)
{
  return (theta * 180.0 / M_PI);
}

extern inline double carmen_degrees_to_radians(double theta)
{
  return (theta * M_PI / 180.0);
}
//---------------



/*
Markus Original

#ifdef __cplusplus
}
#endif
*/
#endif
