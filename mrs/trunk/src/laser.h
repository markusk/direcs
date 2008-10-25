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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <QtGlobal>
#include <QString>
#include <QDebug>

#include "sick.h"


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

#define      CARMEN_LASER_ALIVE_NAME            "carmen_laser_alive"
#define      CARMEN_LASER_ALIVE_FMT             "{int,int,int,int,int}"




class Laser : public QObject
{
    Q_OBJECT
	
	public:
		Laser();
		~Laser();
		
		static const short int LASER1 = 1;	///	For the mrs laserThread
		static const short int LASER2 = 2;	///	For the mrs laserThread
		static const short int LASER3 = 4;	///	For the mrs laserThread
		static const short int LASER4 = 8;	///	For the mrs laserThread
		static const short int LASER5 = 16;	///	For the mrs laserThread
		
		// FROM param_interface.h:
		typedef char carmen_param_type_t;
		// This data structure is used by carmen_param_install_params and
		//    carmen_param_usage to load and report the usage of a large number of
		//    parameters.

		typedef void (*carmen_param_change_handler_t)(char *module, char *variable, char *value);
		
		typedef struct
		{
			char *module;	// <The module name of this parameter
			char *variable;	// <The variable name to be loaded.
			carmen_param_type_t type;	// <Type should match user_variable:
										// e.g., CARMEN_PARAM_INT if the local
										// variable storage is an integer.
			void *user_variable;	// <A pointer to the local variable
									// storage.
			int subscribe;	// <If the param_daemon publishes a
							// change to this variable value (because
							// someone has changed the variable
							// value, should the local value be
							// updated? 1 for yes, 0 for no.
			carmen_param_change_handler_t handler;	// <Declare a handler if the
													// param_daemon publishes a change to
													// this variable's value.
		} carmen_param_t, *carmen_param_p;

		// * * * * *
		typedef enum { PLS, LMS } laser_model_t;
		typedef enum { CM, MM, DM } range_res_t;
		typedef enum { SICK_RANGE80M, SICK_RANGE160M, SICK_RANGE320M, SICK_REMISSION_NORM, SICK_REMISSION_DIRECT } range_dist_t;
		typedef enum { N, E, O } parity_t;
		
		typedef struct
		{
			int                fd;
			laser_model_t      type;
			char               *ttyport;
			int                baudrate;
			parity_t           parity;
			unsigned char      passwd[8];
			int                databits;
			int                stopbits;
			int                hwf;
			int                swf;
			int                laser_num;
		} laser_device_t, *laser_device_p;
		
		typedef struct
		{
			char device_name[MAX_NAME_LENGTH];
			laser_model_t type;
			range_res_t range_res;
			range_dist_t range_dist;
			unsigned char password[8];
			int laser_num;
			int detect_baudrate, use_highspeed;
			int start_baudrate, set_baudrate;
			int databits, stopbits;
			parity_t parity;
			int swf, hwf;
			int angle_range, angle_resolution;
			int use_remission;
			int rem_values;
			int num_values;
			int laser_flipped;
		} laser_settings_t;

		typedef struct
		{
			laser_settings_t settings;
			laser_device_t dev;
			
			int numvalues;
			double *range;
			int *glare, *wfv, *sfv;
			// *** REI - START ***
			double *remission;
			int remvalues;
			// *** REI - END *** //
			unsigned char *buffer;
			long int buffer_position, processed_mark, packet_offset, packet_length;
			int new_reading;
			double timestamp;
			double packet_timestamp;
		} sick_laser_t, *sick_laser_p;
		// * * * * *
		
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
		
		// vorher **global** in laser_main.cpp!
		sick_laser_t *laser1; // defined in sick.h
		sick_laser_t *laser2;
		sick_laser_t *laser3;
		sick_laser_t *laser4;
		sick_laser_t *laser5;
		carmen_laser_laser_config_t *laser1_config;
		carmen_laser_laser_config_t *laser2_config;
		carmen_laser_laser_config_t *laser3_config;
		carmen_laser_laser_config_t *laser4_config;
		carmen_laser_laser_config_t *laser5_config;
		
		int use_laser1;
		int use_laser2;
		int use_laser3;
		int use_laser4;
		int use_laser5;
		int quit_signal;
		
		//Markus (used in "read_parameters")
		QString laserSerialPort1;
		QString laserSerialPort2;
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
		
		Sick *sick;
};

#endif
