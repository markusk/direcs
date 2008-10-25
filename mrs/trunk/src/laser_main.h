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

#include "sick.h"
//#include "laser_messages.h"
#include "param_interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <QtGlobal>
#include <QString>
#include <QDebug>


// from laser.h:
#define     CARMEN_FRONT_LASER_NUM       0
#define     CARMEN_REAR_LASER_NUM        1
#define     CARMEN_LASER3_NUM            2
#define     CARMEN_LASER4_NUM            3
#define     CARMEN_LASER5_NUM            4


// from laser_messages.h
/**
 * Definition of the messages for the module laser.
 *
 * This file specifies the messages for this modules used to transmit
 * data via ipc to other modules.
 **/
/** all raw laser messages have the same format **/
#define      CARMEN_LASER_LASER_FMT       "{int,{int,double,double,double,double,double,int},int,<float:3>,int,<float:5>,double,string}"

#define      CARMEN_LASER_FRONTLASER_NAME "carmen_laser_frontlaser"
#define      CARMEN_LASER_FRONTLASER_FMT  CARMEN_LASER_LASER_FMT

/** Note that laser1 is the same as frontlaser **/
#define      CARMEN_LASER_LASER1_NAME     CARMEN_LASER_FRONTLASER_NAME
#define      CARMEN_LASER_LASER1_FMT      CARMEN_LASER_FRONTLASER_FMT

#define      CARMEN_LASER_REARLASER_NAME  "carmen_laser_rearlaser"
#define      CARMEN_LASER_REARLASER_FMT   CARMEN_LASER_LASER_FMT

/** Note that laser2 is the same as reartlaser **/
#define      CARMEN_LASER_LASER2_NAME     CARMEN_LASER_REARLASER_NAME
#define      CARMEN_LASER_LASER2_FMT      CARMEN_LASER_REARLASER_FMT

#define      CARMEN_LASER_LASER3_NAME     "carmen_laser_laser3"
#define      CARMEN_LASER_LASER3_FMT      CARMEN_LASER_LASER_FMT

#define      CARMEN_LASER_LASER4_NAME     "carmen_laser_laser4"
#define      CARMEN_LASER_LASER4_FMT      CARMEN_LASER_LASER_FMT

#define      CARMEN_LASER_LASER5_NAME     "carmen_laser_laser5"
#define      CARMEN_LASER_LASER5_FMT      CARMEN_LASER_LASER_FMT

/* Markus:
/ ** configuration of the laser ** /
typedef struct {
  int frontlaser_stalled;  / **< laser1 (frontlaser) is stalled(=1) or is ok(=0) ** /
  int rearlaser_stalled;   / **< laser1 (rearlaser) is stalled(=1) or is ok(=0) ** /
  int laser3_stalled;      / **< laser3 is stalled(=1) or is ok(=0) ** /
  int laser4_stalled;      / **< laser4 is stalled(=1) or is ok(=0) ** /
  int laser5_stalled;      / **< laser5 is stalled(=1) or is ok(=0) ** /
} carmen_laser_alive_message;
*/
#define      CARMEN_LASER_ALIVE_NAME            "carmen_laser_alive"
#define      CARMEN_LASER_ALIVE_FMT             "{int,int,int,int,int}"


class Laser : public QObject
{
    Q_OBJECT
	
	public:
		Laser();
		~Laser();
		
		int carmen_laser_start(short int laserScanner);
		void carmen_laser_shutdown(int x);
		int carmen_laser_run(void);
		void shutdown_laser(int x);
		int getLaserNumReadings(int laser);
		float getLaserDistance(int laser, int angle);
		void setDevicePort(short int laser, QString serialPort);
		double carmen_radians_to_degrees(double theta);
		double carmen_degrees_to_radians(double theta);
		void  set_laser_config_structure(sick_laser_p laser, carmen_laser_laser_config_t* config);
		void read_parameters(short int laserScanner);
		void interpret_params(sick_laser_p laser, char *dev, char *type, double res, char *rem, double fov);
		void check_parameter_settings(sick_laser_p laser);
		void set_default_parameters(sick_laser_p laser, int laser_num);
		
		static const short int LASER1 = 1;	///	For the mrs laserThread
		static const short int LASER2 = 2;	///	For the mrs laserThread
		static const short int LASER3 = 4;	///	For the mrs laserThread
		static const short int LASER4 = 8;	///	For the mrs laserThread
		static const short int LASER5 = 16;	///	For the mrs laserThread
		
		// FROM laser_messages.h:
		/** defined the type of laser that is used **/
		typedef enum
		{
			SICK_LMS = 0,
			SICK_PLS = 1,
			HOKUYO_URG = 2,
			SIMULATED_LASER = 3,
			UMKNOWN_PROXIMITY_SENSOR = 99
		} carmen_laser_laser_type_t;
		
		/** Possible remission values **/
		typedef enum
		{
			REMISSION_NONE       = 0,
			REMISSION_DIRECT     = 1,
			REMISSION_NORMALIZED = 2
		} carmen_laser_remission_type_t;
		
		/** The configuration of the laser  **/
		typedef struct
		{
			carmen_laser_laser_type_t laser_type;   /**< what kind of laser is this **/
			double start_angle;                     /**< angle of the first beam relative to the center of the laser **/
			double fov;                             /**< field of view of the laser **/
			double angular_resolution;              /**< angular resolution of the laser **/
			double maximum_range;                   /**< the maximum valid range of a measurement  **/
			double accuracy;                        /**< error in the range measurements **/
			carmen_laser_remission_type_t remission_mode;  /** if and what kind of remission values are used */
		} carmen_laser_laser_config_t;
		
		/* Markus:
		/ ** The laser message of the laser module (rawlaser) ** /
		typedef struct {
			int id;                                / **< id of the laser (FLASER, RLASER, etc) ** /
			carmen_laser_laser_config_t config;    / **< configuration of the laser ** /
			int num_readings;                      / **< the number of beams of this laser message ** /
			float *range;                          / **< the measured distances (in meters) ** /
			int num_remissions;                    / **< the number of remission values. Should be 0 if config.remission_mode is OFF ** /
			float *remission;                      / **< the remission values ** /
			double timestamp;                      / **< timestamp when the message was read from the laser (typically the serial line) ** /
			char *host;                            / **< the host to which the laser is connected  ** /
		} carmen_laser_laser_message ;
		*/
		
		// vorher **global** in laser_main.cpp!
		sick_laser_t laser1; // defined in sick.h
		sick_laser_t laser2;
		sick_laser_t laser3;
		sick_laser_t laser4;
		sick_laser_t laser5;
		carmen_laser_laser_config_t laser1_config;
		carmen_laser_laser_config_t laser2_config;
		carmen_laser_laser_config_t laser3_config;
		carmen_laser_laser_config_t laser4_config;
		carmen_laser_laser_config_t laser5_config;
		
		int use_laser1;
		int use_laser2;
		int use_laser3;
		int use_laser4;
		int use_laser5;
		int quit_signal;
		
		//Markus (used in "read_parameters")
		QString laserSerialPort1;
		QString laserSerialPort2;
}

#endif
