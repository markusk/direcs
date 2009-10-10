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

#ifndef LASER_H
#define LASER_H

#include "direcsSerial.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/utsname.h>

#if !defined(CYGWIN) && !defined(__APPLE__)
#include <linux/serial.h>
#include <linux/version.h>
#endif

#ifdef CYGWIN
#include <sys/socket.h>
#endif

#ifndef va_copy
#define va_copy __va_copy
#endif

#ifdef __USE_BSD
#undef __USE_BSD
#include <string.h>
#define __USE_BSD
#else
#include <string.h>
#endif


#ifndef MAXDOUBLE
#define MAXDOUBLE DBL_MAX
#endif
#ifndef MAXFLOAT
#define MAXFLOAT FLT_MAX
#endif

#define     DIRECS_FRONT_LASER_NUM       0
#define     DIRECS_REAR_LASER_NUM        1
#define     DIRECS_LASER3_NUM            2
#define     DIRECS_LASER4_NUM            3
#define     DIRECS_LASER5_NUM            4


/**
 * Definition of the messages for the module laser.
 *
 * This file specifies the messages for this modules used to transmit
 * data via ipc to other modules.
 **/
/// all raw laser messages have the same format
#define      DIRECS_LASER_LASER_FMT       "{int,{int,double,double,double,double,double,int},int,<float:3>,int,<float:5>,double,string}"

#define      DIRECS_LASER_FRONTLASER_NAME "direcs_laser_frontlaser"
#define      DIRECS_LASER_FRONTLASER_FMT  DIRECS_LASER_LASER_FMT

/// Note that laser1 is the same as frontlaser
#define      DIRECS_LASER_LASER1_NAME     DIRECS_LASER_FRONTLASER_NAME
#define      DIRECS_LASER_LASER1_FMT      DIRECS_LASER_FRONTLASER_FMT

#define      DIRECS_LASER_REARLASER_NAME  "direcs_laser_rearlaser"
#define      DIRECS_LASER_REARLASER_FMT   DIRECS_LASER_LASER_FMT

/// Note that laser2 is the same as reartlaser
#define      DIRECS_LASER_LASER2_NAME     DIRECS_LASER_REARLASER_NAME
#define      DIRECS_LASER_LASER2_FMT      DIRECS_LASER_REARLASER_FMT

#define      DIRECS_LASER_LASER3_NAME     "direcs_laser_laser3"
#define      DIRECS_LASER_LASER3_FMT      DIRECS_LASER_LASER_FMT

#define      DIRECS_LASER_LASER4_NAME     "direcs_laser_laser4"
#define      DIRECS_LASER_LASER4_FMT      DIRECS_LASER_LASER_FMT

#define      DIRECS_LASER_LASER5_NAME     "direcs_laser_laser5"
#define      DIRECS_LASER_LASER5_FMT      DIRECS_LASER_LASER_FMT

#define      DIRECS_LASER_ALIVE_NAME            "direcs_laser_alive"
#define      DIRECS_LASER_ALIVE_FMT             "{int,int,int,int,int}"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846  // pi
#endif

#define DIRECS_LASER_USE_SELECT 1
#define DIRECS_LASER_LOW_LATENCY 1

#define LASER_BUFFER_SIZE                100000

#define CRC16_GEN_POL                    0x8005
#define CRC16_GEN_POL0                   0x80
#define CRC16_GEN_POL1                   0x05

#define LMS_PASSWORD                     "SICK_LMS"
#define PLS_PASSWORD                     "SICK_PLS"

#define BUFFER_SIZE                      16000
#define MAX_COMMAND_SIZE                 8196

#define MAX_NAME_LENGTH                  256

#define MAX_TIME_FOR_CLEAR               0.2
#define MAX_TIME_FOR_DATA                0.3
#define MAX_TIME_FOR_ACK                 0.1
#define MAX_TIME_FOR_ANSWER              0.1
#define MAX_TIME_FOR_SENSOR_DATA         0.5
#define MAX_TIME_FOR_CONFIG              3.0
#define MAX_TIME_FOR_GETTING_CONF        0.3
#define MAX_TIME_FOR_TESTING_BAUDRATE    1.0

#define INI                              -1
#define TIO                              0
#define STX                              0x02
#define UKN                              0x05
#define ACK                              0x06
#define DLE                              0x10
#define NAK                              0x15
#define LID                              0x80

#define RES_1_00_DEGREE                  0
#define RES_0_50_DEGREE                  1 
#define RES_0_25_DEGREE                  2

#ifndef TIOCGETP 
#define TIOCGETP                         0x5481
#define TIOCSETP                         0x5482
#define RAW                              1
#define CBREAK                           64
#endif

#include <QtGlobal>
#include <QString>
#include <QDebug>


/**
\brief This class handles all the low level laser stuff for the old laser scanners.

Currently supported are SICK LMS, PLS or HOKUYO_URG.
*/
class Laser : public QObject
{
    Q_OBJECT
	
	public:
		Laser();
		~Laser();
		
		static const short int LASER1 = 1;	/// For the direcs laserThread
		static const short int LASER2 = 2;	/// For the direcs laserThread
		static const short int LASER3 = 4;	/// For the direcs laserThread
		static const short int LASER4 = 8;	/// For the direcs laserThread
		static const short int LASER5 = 16;	/// For the direcs laserThread
		
		/**
		This data structure is used by direcs_param_install_params and
		direcs_param_usage to load and report the usage of a large number of
		parameters.
		*/
		typedef char direcs_param_type_t;

		typedef void (*direcs_param_change_handler_t)(char *module, char *variable, char *value);
		
		typedef struct
		{
			char *module;							/// The module name of this parameter
			char *variable;							/// The variable name to be loaded.
			direcs_param_type_t type;				/// Type should match user_variable:
													/// e.g., DIRECS_PARAM_INT if the local
													/// variable storage is an integer.
			void *user_variable;					/// A pointer to the local variable storage.
			int subscribe;							/// If the param_daemon publishes a
													/// change to this variable value (because
													/// someone has changed the variable
													/// value, should the local value be
													/// updated? 1 for yes, 0 for no.
			direcs_param_change_handler_t handler;	/// Declare a handler if the
													/// param_daemon publishes a change to
													/// this variable's value.
		} direcs_param_t, *direcs_param_p;

		typedef enum { PLS, LMS } laser_model_t;
		typedef enum { CM, MM, DM } range_res_t;
		typedef enum { SICK_RANGE80M, SICK_RANGE160M, SICK_RANGE320M, SICK_REMISSION_NORM, SICK_REMISSION_DIRECT } range_dist_t;
		typedef enum { N, E, O } parity_t;
		
		typedef struct
		{
			int                fd; // this is the file descriptor
			laser_model_t      type;
			QString            ttyport;
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
			QString device_name;
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
			double *remission;
			int remvalues;
			unsigned char *buffer;
			long int buffer_position, processed_mark, packet_offset, packet_length;
			int new_reading;
			double timestamp;
			double packet_timestamp;
		} sick_laser_t, *sick_laser_p;
		
		/**
		defines the type of laser that is used
		*/
		typedef enum
		{
			SICK_LMS = 0,
			SICK_PLS = 1,
			HOKUYO_URG = 2,
			SIMULATED_LASER = 3,
			UMKNOWN_PROXIMITY_SENSOR = 99
		} direcs_laser_laser_type_t;
		
		/**
		Possible remission values
		*/
		typedef enum
		{
			REMISSION_NONE       = 0,
			REMISSION_DIRECT     = 1,
			REMISSION_NORMALIZED = 2
		} direcs_laser_remission_type_t;
		
		/**
		The configuration of the laser
		*/
		typedef struct
		{
			direcs_laser_laser_type_t laser_type;			/// what kind of laser is this
			double start_angle;								/// angle of the first beam relative to the center of the laser
			double fov;										/// field of view of the laser
			double angular_resolution;						/// angular resolution of the laser
			double maximum_range;							/// the maximum valid range of a measurement
			double accuracy;								/// error in the range measurements
			direcs_laser_remission_type_t remission_mode;	/// if and what kind of remission values are used
		} direcs_laser_laser_config_t;
		
		// vorher **global** in laser_main.cpp!
		sick_laser_t *laser1; // defined in sick.h
		sick_laser_t *laser2;
		sick_laser_t *laser3;
		sick_laser_t *laser4;
		sick_laser_t *laser5;
		direcs_laser_laser_config_t *laser1_config;
		direcs_laser_laser_config_t *laser2_config;
		direcs_laser_laser_config_t *laser3_config;
		direcs_laser_laser_config_t *laser4_config;
		direcs_laser_laser_config_t *laser5_config;
		
		int use_laser1;
		int use_laser2;
		int use_laser3;
		int use_laser4;
		int use_laser5;
		int quit_signal;
		
		QString laserSerialPort1;
		QString laserSerialPort2;
		QString laserSerialPort3;
		QString laserSerialPort4;
		QString laserSerialPort5;
		int direcs_laser_start(short int laserScanner);
		void direcs_laser_shutdown(int x);
		int direcs_laser_run(void);
		void shutdown_laser(int x);
		int getLaserNumReadings(int laser);
		float getLaserDistance(int laser, int angle);
		void setDevicePort(short int laser, QString serialPort);
		double direcs_radians_to_degrees(double theta);
		double direcs_degrees_to_radians(double theta);
		void  set_laser_config_structure(sick_laser_p laser, direcs_laser_laser_config_t* config);
		void read_parameters(short int laserScanner);
		void interpret_params(sick_laser_p laser, QString dev, QString type, double res, QString rem, double fov);
		void check_parameter_settings(sick_laser_p laser);
		void set_default_parameters(sick_laser_p laser, int laser_num);
		
		/**
		Establish the serial connection to the laser and set the laser in the wanted mode(s).
		*/
		int  sick_start_laser(sick_laser_p laser);
		
		void sick_stop_laser(sick_laser_p laser);
		
		/**
		Process any data that is available from the laser. Attempt to detect valid packets in the data.
		*/
		void sick_handle_laser(sick_laser_p laser);
		
		int sick_connect_device(sick_laser_p laser);
		
		double direcs_get_time(void);


	signals:
		/**
		Emits a info messge to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		*/
		void message(QString text);


	private:
		int iParity(parity_t par);
		int iSoftControl(int flowcontrol);
		int cDataSize(int numbits);
		int cStopSize(int numbits);
		int cFlowControl(int flowcontrol);
		int cParity(parity_t par);
		int cBaudrate(int baudrate);
		void sick_set_serial_params(sick_laser_p laser);
		int kernel_minimum_version( int a, int b, int c );
		void sick_set_baudrate(sick_laser_p laser, int brate);
		
		/**
		This method opens the serial port of the laser scanner, sets the serial port parameters and tries to set the latency mode, if defined
		@return the file descriptor of the opend serial port
		*/
		int sick_serial_connect(sick_laser_p laser);
		
		/**
		Compute the CRC checksum of a segment of data.
		*/
		int sick_compute_checksum(unsigned char *CommData, int uLen);
		
		int sick_read_data(sick_laser_p laser, unsigned char *data, double timeout);
		int sick_write_command(sick_laser_p laser, unsigned char command, unsigned char *argument, int arg_length);
		void sick_request_status(sick_laser_p laser);
		void sick_request_sensor(sick_laser_p laser);
		int sick_set_laser_baudrate(sick_laser_p laser, int brate);
		int sick_set_config_mode(sick_laser_p laser);
		int sick_set_lms_resolution(sick_laser_p laser);
		int sick_request_lms_config(sick_laser_p laser);
		int sick_set_lms_config(sick_laser_p laser, unsigned char *data, int len);
		int sick_parse_conf_data(sick_laser_p laser, unsigned char *buf, int length);
		int sick_set_lms_range(sick_laser_p laser);
		void sick_start_continuous_mode(sick_laser_p laser);
		void sick_stop_continuous_mode(sick_laser_p laser);
		void sick_start_continuous_remission_part_mode(sick_laser_p laser);
		
		/**
		Test a combination of baudrate and parity of the laser.
		*/
		int sick_testBaudrate(sick_laser_p laser, int brate);
		
		int sick_detect_baudrate(sick_laser_p laser);
		
		int sick_check_baudrate(sick_laser_p laser, int brate);
		
		void sick_install_settings(sick_laser_p laser);
		
		void sick_allocate_laser(sick_laser_p laser);
		
		/**
		This function returns 1 if a valid packet is detected in a chunk of data.
		@return An offset and packet length
		*/
		int sick_valid_packet(unsigned char *data, long size, long *offset, long *len);
		
		/**
		Interpret packets received from the laser.  If the packets contain laser data, expand the data into a useful form.
		*/
		void sick_process_packet_distance(sick_laser_p laser, unsigned char *packet);
		
		void sick_process_packet_remission(sick_laser_p laser, unsigned char *packet);
		
		void sick_process_packet(sick_laser_p laser, unsigned char *packet);
		
		DirecsSerial *serialPort;
};

#endif
