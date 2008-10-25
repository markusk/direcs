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

#ifndef CARMEN_SICK_H
#define CARMEN_SICK_H

#include <QtGlobal>

// FROM carmen/carmen.h:
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef va_copy
#define va_copy __va_copy
#endif 

#include <unistd.h>
#include <ctype.h>
#ifdef __USE_BSD
#undef __USE_BSD
#include <string.h>
#define __USE_BSD
#else
#include <string.h>
#endif
#include <signal.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef CYGWIN
#include <sys/socket.h>
#endif
#include <errno.h>
#include <limits.h>
#include <float.h>
#ifndef MAXDOUBLE
#define MAXDOUBLE DBL_MAX
#endif
#ifndef MAXFLOAT
#define MAXFLOAT FLT_MAX
#endif


// FROM global.h:
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846  /* pi */
#endif

// FROM rflex-io.c:
#include <termios.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#ifdef CYGWIN
#include <sys/socket.h>
#endif
// - - - - - - - - - - - - -

#define CARMEN_LASER_USE_SELECT 1
#define CARMEN_LASER_LOW_LATENCY 1


#include "carmenserial.h"

#include <sys/types.h>
#include <sys/select.h>

/*****  DIRK WAS HERE - START ******/
#include <sys/utsname.h>
#if !defined(CYGWIN) && !defined(__APPLE__)
#include <linux/serial.h>
#include <linux/version.h>
#endif
/*****  DIRK WAS HERE - END ******/



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

/**
\brief Handles the access to the sick laser scanners.

This class handles the access to the sick laser scanners.
*/
class Sick : public QObject
{
    Q_OBJECT
	
	public:
		Sick();
		~Sick();
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
		
		int  sick_start_laser(sick_laser_p laser);
		void sick_stop_laser(sick_laser_p laser);
		void sick_handle_laser(sick_laser_p laser);
		int sick_connect_device(sick_laser_p laser);
		double carmen_get_time(void); // from global.c
		
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
		int sick_serial_connect(sick_laser_p laser);
		static int sick_compute_checksum(unsigned char *CommData, int uLen);
		int sick_read_data(sick_laser_p laser, unsigned char *data, double timeout);
		int sick_write_command(sick_laser_p laser, unsigned char command, unsigned char *argument, int arg_length);
		void sick_request_status(sick_laser_p laser);
		void sick_request_sensor(sick_laser_p laser);
		int sick_set_laser_baudrate(sick_laser_p laser, int brate);
		int sick_set_config_mode(sick_laser_p laser);
		int sick_set_lms_resolution(sick_laser_p laser);
		int sick_request_lms_config(sick_laser_p laser);
		int sick_parse_conf_data(sick_laser_p laser, unsigned char *buf, int length);
		int sick_set_lms_range(sick_laser_p laser);
		void sick_start_continuous_mode(sick_laser_p laser);
		void sick_stop_continuous_mode(sick_laser_p laser);
		void sick_start_continuous_remission_part_mode(sick_laser_p laser);
		int sick_testBaudrate(sick_laser_p laser, int brate);
		int sick_detect_baudrate(sick_laser_p laser);
		int sick_check_baudrate(sick_laser_p laser, int brate);
		void sick_install_settings(sick_laser_p laser);
		void sick_allocate_laser(sick_laser_p laser);
		int sick_valid_packet(unsigned char *data, long size, long *offset, long *len);
		static void sick_process_packet_distance(sick_laser_p laser, unsigned char *packet);
		static void sick_process_packet_remission(sick_laser_p laser, unsigned char *packet);
		static void sick_process_packet(sick_laser_p laser, unsigned char *packet);
}

#endif
