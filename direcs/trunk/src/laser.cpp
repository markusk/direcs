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
 *   mrs is distributed in the hope that it will be useful,              *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with mrs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                       *
 *************************************************************************/

#include "laser.h"

Laser::Laser()
{
	use_laser1 = 0;
	use_laser2 = 0;
	use_laser3 = 0;
	use_laser4 = 0;
	use_laser5 = 0;
	quit_signal = 0;
	
	laser1 = new sick_laser_t;
	laser2 = new sick_laser_t;
	laser3 = new sick_laser_t;
	laser4 = new sick_laser_t;
	laser5 = new sick_laser_t;
		
	laser1_config = new carmen_laser_laser_config_t;
	laser2_config = new carmen_laser_laser_config_t;
	laser3_config = new carmen_laser_laser_config_t;
	laser4_config = new carmen_laser_laser_config_t;
	laser5_config = new carmen_laser_laser_config_t;
	
	serialPort = new DirecsSerial();
}


Laser::~Laser()
{
	delete serialPort;
	
	delete laser1_config;
	delete laser2_config;
	delete laser3_config;
	delete laser4_config;
	delete laser5_config;
	
	delete laser1;
	delete laser2;
	delete laser3;
	delete laser4;
	delete laser5;
}


void Laser::set_default_parameters(sick_laser_p laser, int laser_num)
{
	laser->settings.type = LMS;
	laser->settings.range_res = CM;
	laser->settings.range_dist = SICK_RANGE80M;
	laser->settings.laser_num = laser_num;
	strcpy(laser->settings.device_name, "/dev/tty0"); // only default! real value set in read_parameters!
	laser->settings.detect_baudrate = TRUE;
	laser->settings.use_highspeed = FALSE;
	laser->settings.start_baudrate = 9600;
	laser->settings.set_baudrate = 38400;
	laser->settings.databits = 8;
	laser->settings.parity = N;
	laser->settings.stopbits = 1;
	laser->settings.hwf = 0;
	laser->settings.swf = 0;
	laser->settings.angle_range = 180;
	laser->settings.angle_resolution = RES_1_00_DEGREE;
	laser->settings.laser_flipped = 1;
	laser->settings.use_remission = 0;
}


void Laser::check_parameter_settings(sick_laser_p laser)
{
	/*********************** TYPE CHECKING **************************/
	if(laser->settings.type == PLS) {
		strncpy((char *)laser->settings.password, (const char *)PLS_PASSWORD, 8);
		laser->settings.parity = E;
	} 
	if(laser->settings.type == LMS) {
		strncpy((char *)laser->settings.password, (const char *)LMS_PASSWORD, 8);
		laser->settings.parity = N;
	} 
	
	/*********************** START BAUDRATE **************************/
	if(laser->settings.detect_baudrate)
		laser->settings.start_baudrate = 9600;
	else if(laser->settings.start_baudrate != 9600 &&
		laser->settings.start_baudrate != 19200 &&
		laser->settings.start_baudrate != 38400 &&
		laser->settings.start_baudrate != 500000) {
		fprintf(stderr, "ERROR: start baudrate = %d is not valid!\n",
			laser->settings.start_baudrate);
		exit(1);
	}
	
	/*********************** SET BAUDRATE **************************/
	if(laser->settings.set_baudrate != 9600 &&
		laser->settings.set_baudrate != 19200 &&
		laser->settings.set_baudrate != 38400 &&
		laser->settings.set_baudrate != 500000) {
		fprintf(stderr, "ERROR: set baudrate = %d is not valid!\n",
			laser->settings.set_baudrate);
		exit(1);
	} 
	else if(laser->settings.set_baudrate == 500000)
		laser->settings.use_highspeed = TRUE;
	
	/*********************** NUM VALUES **************************/
	if(laser->settings.angle_range != 180 && 
		laser->settings.angle_range != 100) {
		fprintf(stderr, "ERROR: angle range = %d is not valid!\n",
			laser->settings.angle_range);
		exit(1);
	}
	
	/************************** ANGLE RANGE ************************/
	if(laser->settings.angle_range == 100) {
		if(laser->settings.angle_resolution == RES_1_00_DEGREE)
		laser->settings.num_values = 101;
		else if(laser->settings.angle_resolution == RES_0_50_DEGREE)
		laser->settings.num_values = 201;
		else if(laser->settings.type == LMS)
		laser->settings.num_values = 401;
		else
		fprintf(stderr, "ERROR: ang-res=0.25 is not valid for this laser!\n");
	}
	else {
		if(laser->settings.angle_resolution == RES_1_00_DEGREE)
		laser->settings.num_values = 181;
		else if(laser->settings.angle_resolution == RES_0_50_DEGREE)
		laser->settings.num_values = 361;
		else {
		fprintf(stderr, "ERROR: ang-res=0.25 and ang-range=180 is not valid!\n");
		exit(1);
		}
	}
	if(laser->settings.type == PLS) {
		if(laser->settings.angle_range == 100) {
		fprintf(stderr, "ERROR: type = PLS and ang-range=100 is not valid!\n");
		exit(1);
		} 
	}
	
	
	/********************** REMISSION RANGE ************************/
	/* remission values - start */
	if (laser->settings.use_remission == 1 && 
		laser->settings.type != LMS) {
		fprintf(stderr, "ERROR: remission values are only available using LMS laser!\n");
		exit(1);
	}
	if (laser->settings.use_remission == 1 && 
		laser->settings.angle_resolution != RES_1_00_DEGREE) {
		fprintf(stderr, "ERROR: remission values are only available with 1.0 degree resolution!\n");
		exit(1);
	}
	
	if(laser->settings.use_remission == 1) 
		laser->settings.rem_values = laser->settings.num_values;
	else 
		laser->settings.rem_values = 0;
	/* remission values - stop */
}


void Laser::interpret_params(sick_laser_p laser, char *dev, char *type, double res, char *rem, double fov)
{
	strcpy(laser->settings.device_name, dev);
	
	if(strcmp(type, "LMS") == 0)
		laser->settings.type = LMS;
	else if(strcmp(type, "PLS") == 0)
		laser->settings.type = PLS;

	if (fabs(fov-M_PI) < 0.1 || fabs(fov-100.0/180.0*M_PI) < 0.1)
		qDebug("The parameter laser_laserX_fov in the ini file must\nbe specified in degrees not in radians!");

	// Markus Original: laser->settings.angle_range = carmen_round(fov);
	laser->settings.angle_range = qRound(fov);

	if ( laser->settings.angle_range != 180 && laser->settings.angle_range != 100 )
		qDebug("The laser driver only provides 180 deg and 100 deg field of view!");

	if(res == 0.25) {
		laser->settings.angle_resolution = RES_0_25_DEGREE;
		laser->settings.angle_range = 100;
	}
	else if(res == 0.5)
		laser->settings.angle_resolution = RES_0_50_DEGREE;
	else
		laser->settings.angle_resolution = RES_1_00_DEGREE;
	
	/* remission values - start */
	if(strcmp(rem, "direct") == 0) {
		laser->settings.use_remission = 1;
		laser->settings.range_dist = SICK_REMISSION_DIRECT;
	}
	else if(strcmp(rem, "normalized") == 0) {
		laser->settings.use_remission = 1;
		laser->settings.range_dist = SICK_REMISSION_NORM;
	}
	else if(strcmp(rem, "no") == 0) {
		laser->settings.use_remission = 0;
	}
	else if(strcmp(rem, "off") == 0) {
		laser->settings.use_remission = 0;
		qDebug("Warning: please set the value of the parameter \nlaser_use_remission to \"no\" and do not use \"off\".\nAssuming \"no\" for now.\n\n");
	}
	else qDebug("ERROR: Parameter laser_use_remission for laser %d has invalid value: %s\nPossible values are: direct, normalized and off.\n", laser->settings.laser_num, rem);
	/* remission values - stop */
	
	check_parameter_settings(laser);
}


void Laser::read_parameters(short int laserScanner)
{
	char *dev1, *dev2, *dev3, *dev4, *dev5;
	char *str1, *str2, *str3, *str4, *str5;
	double res1, res2, res3, res4, res5;
	double fov1, fov2, fov3, fov4, fov5;
	char *rem1, *rem2, *rem3, *rem4, *rem5;
	
	// Markus
	// for QString to char* conversion
	QByteArray ba;
	
	
	switch (laserScanner)
	{
		case LASER1:
			ba = laserSerialPort1.toLatin1();
			dev1 = ba.data();
			str1 = "PLS";
			res1 = 1.0;
			rem1 = "no";
			fov1 = 180;
			if(strncmp(dev1, "none", 4) != 0)
			{
				use_laser1 = 1;
				interpret_params(laser1, dev1, str1, res1, rem1, fov1);
			}
			break;
		case LASER2:
			ba = laserSerialPort2.toLatin1();
			dev2 = ba.data();
			str2 = "PLS";
			res2 = 1.0;
			rem2 = "no";
			fov2 = 180;
			if(strncmp(dev2, "none", 4) != 0)
			{
				use_laser2 = 1;
				interpret_params(laser2, dev2, str2, res2, rem2, fov2);
			}
			break;
		case LASER3:
			dev3 = "none";
			str3 = "PLS";
			res3 = 1.0;
			rem3 = "no";
			fov3 = 180;
			if(strncmp(dev3, "none", 4) != 0)
			{
				use_laser3 = 1;
				interpret_params(laser3, dev3, str3, res3, rem3, fov3);
			}
			break;
		case LASER4:
			dev4 = "none";
			str4 = "PLS";
			res4 = 1.0;
			rem4 = "no";
			fov4 = 180;
			if(strncmp(dev4, "none", 4) != 0)
			{
				use_laser4 = 1;
				interpret_params(laser4, dev4, str4, res4, rem4, fov4);
			}
			break;
		case LASER5:
			dev5 = "none";
			str5 = "PLS";
			res5 = 1.0;
			rem5 = "no";
			fov5 = 180;
			if(strncmp(dev5, "none", 4) != 0)
			{
				use_laser5 = 1;
				interpret_params(laser5, dev5, str5, res5, rem5, fov5);
			}
			break;
	}
}


void  Laser::set_laser_config_structure(sick_laser_p laser, carmen_laser_laser_config_t* config)
{

	if (laser->settings.type == LMS) {
		config->laser_type = SICK_LMS;
		config->maximum_range = 81.90;
	
		if (laser->settings.range_res == MM)
		config->accuracy = 0.035;  /* 5cm in cm mode, 35mm in mm mode */
		else
		config->accuracy = 0.05;   /* 5cm in cm mode, 35mm in mm mode */
	}
	else if (laser->settings.type == PLS) {
		config->laser_type = SICK_PLS;
		config->maximum_range = 50.0;
		config->accuracy = 0.15; /* I need to look up this value in the SICK specs */
	}
	else { 
		// if unknown, assume LMS
		config->laser_type = SICK_LMS;
		config->maximum_range = 81.90;
	
		if (laser->settings.range_res == MM)
		config->accuracy = 0.035;  /* 5cm in cm mode, 35mm in mm mode */
		else
		config->accuracy = 0.05;   /* 5cm in cm mode, 35mm in mm mode */
	}
	
	if (laser->settings.num_values == 181 ) {
		config->angular_resolution = carmen_degrees_to_radians(1.0); 
		config->fov  = M_PI;
		config->start_angle = -0.5*config->fov;
	} 
	else if (laser->settings.num_values == 361 ) {
		config->angular_resolution = carmen_degrees_to_radians(0.5); 
		config->fov  = M_PI;
		config->start_angle = -0.5*config->fov;
	} 
	else if (laser->settings.num_values == 180 ) {
		config->angular_resolution = carmen_degrees_to_radians(1.0);
		config->fov  = M_PI - config->angular_resolution;
		config->start_angle = -0.5*M_PI;
	} 
	else if (laser->settings.num_values == 360 ) {
		config->angular_resolution = carmen_degrees_to_radians(0.5);
		config->fov  = M_PI - config->angular_resolution;
		config->start_angle = -0.5*M_PI;
	} 
	else   if (laser->settings.num_values == 401 ) {
		config->angular_resolution = carmen_degrees_to_radians(0.25);
		config->fov  = carmen_degrees_to_radians(100.0);
		config->start_angle = -0.5*carmen_degrees_to_radians(100.0);
	} 
	else   if (laser->settings.num_values == 201 ) {
		config->angular_resolution = carmen_degrees_to_radians(0.5);
		config->fov  = carmen_degrees_to_radians(100.0);
		config->start_angle = -0.5*carmen_degrees_to_radians(100.0);
	} 
	else   if (laser->settings.num_values == 101 ) {
		config->angular_resolution = carmen_degrees_to_radians(1.0);
		config->fov  = carmen_degrees_to_radians(100.0);
		config->start_angle = -0.5*carmen_degrees_to_radians(100.0);
	} 
	else   if (laser->settings.num_values == 400 ) {
		config->angular_resolution = carmen_degrees_to_radians(0.25);
		config->fov  = carmen_degrees_to_radians(100.0) - config->angular_resolution;
		config->start_angle = -0.5*carmen_degrees_to_radians(100.0);
	} 
	else   if (laser->settings.num_values == 200 ) {
		config->angular_resolution = carmen_degrees_to_radians(0.5);
		config->fov  = carmen_degrees_to_radians(100.0) - config->angular_resolution;
		config->start_angle = -0.5*carmen_degrees_to_radians(100.0);
	} 
	else   if (laser->settings.num_values == 100 ) {
		config->angular_resolution = carmen_degrees_to_radians(1.0);
		config->fov  = carmen_degrees_to_radians(100.0) - config->angular_resolution;
		config->start_angle = -0.5*carmen_degrees_to_radians(100.0);
	} 
	else {
		config->fov  = M_PI;
		config->start_angle = -0.5*config->fov;
		config->angular_resolution = config->fov/((double)laser->settings.laser_num-1.0);
	
		qDebug("Unkown laser config for a SICK with %d beams\n", laser->settings.num_values);
		qDebug("Guessing: (fov=%.3f, start=%.2f, res=%.2f)\n", config->fov, config->start_angle, config->angular_resolution);
	
	}
	
	if (laser->settings.use_remission == 1 && 
		laser->settings.range_dist == SICK_REMISSION_DIRECT) 
		config->remission_mode = REMISSION_DIRECT;
	else   if (laser->settings.use_remission == 1 && 
		laser->settings.range_dist == SICK_REMISSION_NORM) 
		config->remission_mode = REMISSION_NORMALIZED;
	else
		config->remission_mode = REMISSION_NONE;
}


int Laser::carmen_laser_start(short int laserScanner)
{
	switch (laserScanner)
	{
		case LASER1:
			// get laser default parameter
			set_default_parameters(laser1, CARMEN_FRONT_LASER_NUM);
			// read parameters and also sets "user_laser*"
			read_parameters(LASER1);
			// start lasers, and start publishing scans
			if (use_laser1)
			{
				set_laser_config_structure(laser1, laser1_config);
				
				// try to connect to the laser
				if (sick_start_laser(laser1) == 0)
				{
					return 0;
				}
				else
				{
					// don't use this laser, because of connect error
					use_laser1 = 0;
					return 1;
				}
			}
			break;

		case LASER2:
			// get laser default parameter
			set_default_parameters(laser2, CARMEN_REAR_LASER_NUM);
			// read parameters and also sets "user_laser*"
			read_parameters(LASER2);
			// start lasers, and start publishing scans
			if (use_laser2)
			{
				set_laser_config_structure(laser2, laser2_config);
				
				// try to connect to the laser
				if (sick_start_laser(laser2) == 0)
				{
					return 0;
				}
				else
				{
					// don't use this laser, because of connect error
					use_laser2 = 0;
					return 1;
				}
			}
			break;

		case LASER3:
			// get laser default parameter
			set_default_parameters(laser3, CARMEN_LASER3_NUM);
			// read parameters and also sets "user_laser*"
			read_parameters(LASER3);
			// start lasers, and start publishing scans
			if (use_laser3)
			{
				set_laser_config_structure(laser3, laser3_config);
				
				// try to connect to the laser
				if (sick_start_laser(laser3) == 0)
				{
					return 0;
				}
				else
				{
					// don't use this laser, because of connect error
					use_laser3 = 0;
					return 1;
				}
			}
			break;

		case LASER4:
			// get laser default parameter
			set_default_parameters(laser4, CARMEN_LASER4_NUM);
			// read parameters and also sets "user_laser*"
			read_parameters(LASER4);
			// start lasers, and start publishing scans
			if (use_laser4)
			{
				set_laser_config_structure(laser4, laser4_config);
				
				// try to connect to the laser
				if (sick_start_laser(laser4) == 0)
				{
					return 0;
				}
				else
				{
					// don't use this laser, because of connect error
					use_laser4 = 0;
					return 1;
				}
			}
			break;

		case LASER5:
			// get laser default parameter
			set_default_parameters(laser5, CARMEN_LASER4_NUM);
			// read parameters and also sets "user_laser*"
			read_parameters(LASER5);
			// start lasers, and start publishing scans
			if (use_laser5)
			{
				set_laser_config_structure(laser5, laser5_config);
				
				// try to connect to the laser
				if (sick_start_laser(laser5) == 0)
				{
					return 0;
				}
				else
				{
					// don't use this laser, because of connect error
					use_laser5 = 0;
					return 1;
				}
			}
			break;
	}
	
	// this line should never be reached
	return -1;
}


void Laser::carmen_laser_shutdown(int signo __attribute__ ((unused)))
{
	if(use_laser1)
		sick_stop_laser(laser1);

	if(use_laser2)
		sick_stop_laser(laser2);

	if(use_laser3)
		sick_stop_laser(laser3);

	if(use_laser4)
		sick_stop_laser(laser4);

	if(use_laser5)
		sick_stop_laser(laser5);
}


int Laser::carmen_laser_run(void)
{
	static int first = 1;
	static double last_update;
	static double last_alive = 0;
	double current_time;
	int print_stats;
	static int laser1_stalled = 0, laser2_stalled = 0, laser3_stalled = 0, laser4_stalled = 0, laser5_stalled = 0;;
	int laserValue = 0;

	
	if (first)
	{
		last_update = carmen_get_time();
		first = 0;
	}
	
	current_time = carmen_get_time();
	print_stats = (current_time - last_update > 1.0);

	
	if (use_laser1)
	{
		sick_handle_laser(laser1);
		
		if (laser1->new_reading)
		{
			laserValue += LASER1;
		}
		
		laser1_stalled = (current_time - laser1->timestamp > 1.0);
		//**
		//fprintf(stderr, "time: %.1f",current_time - laser1.timestamp);
		//**
		
		/* Markus Original:
		if (print_stats)
			fprintf(stderr, "L1: %s(%.1f%% full) ", laser1_stalled ? "STALLED " : " ", (laser1.buffer_position - laser1.processed_mark) / (float)LASER_BUFFER_SIZE * 100.0);
		*/
	}
	
	
	if(use_laser2)
	{
		//qDebug("carmen_laser_run: use_laser1=true");
		sick_handle_laser(laser2);
		
		if (laser2->new_reading)
		{
			laserValue += LASER2;
		}
		
		laser2_stalled = (current_time - laser2->timestamp > 1.0);
		/*
		if (print_stats)
			fprintf(stderr, "L2: %s(%.1f%% full) ", laser2_stalled ? "STALLED " : " ", (laser2.buffer_position - laser2.processed_mark) / (float)LASER_BUFFER_SIZE * 100.0);
		*/
	}
	
	
	if( use_laser3)
	{
		sick_handle_laser(laser3);
		
		if (laser3->new_reading)
		{
			laserValue += LASER3;
		}
		
		laser3_stalled = (current_time - laser3->timestamp > 1.0);
		
		/*
		if (print_stats)
			fprintf(stderr, "L3: %s(%.1f%% full) ", laser3_stalled ? "STALLED " : " ", laser3->buffer_position / (float)LASER_BUFFER_SIZE * 100.0);
		*/
	}
	
	
	if (use_laser4)
	{
		sick_handle_laser(laser4);
		
		if (laser4->new_reading)
		{
			laserValue += LASER4;
		}
		
		laser4_stalled = (current_time - laser4->timestamp > 1.0);

		/*
		if(print_stats)
			fprintf(stderr, "L4: %s(%.1f%% full) ", laser4_stalled ? "STALLED " : " ", laser4.buffer_position / (float)LASER_BUFFER_SIZE * 100.0);
		*/
	}
	
	
	if (use_laser5)
	{
		sick_handle_laser(laser5);
		
		if(laser5->new_reading)
		{
			laserValue += LASER5;
		}
		
		laser5_stalled = (current_time - laser5->timestamp > 1.0);
		
		/*
		if(print_stats)
			fprintf(stderr, "L5: %s(%.1f%% full) ", laser5_stalled ? "STALLED " : " ", laser5.buffer_position / (float)LASER_BUFFER_SIZE * 100.0);
		*/
	}
	
	/* Markus Original:
	if (print_stats)
	{
		fprintf(stderr, "\n");
		last_update = current_time;
	}
	*/
	
	if(current_time - last_alive > 1.0)
	{
		last_alive = current_time;
	}

	//carmen_publish_heartbeat("laser");

	return laserValue;
}


void Laser::shutdown_laser(int x)
{
	carmen_laser_shutdown(x);
	exit(-1);
}


int Laser::getLaserNumReadings(int laser)
{
	switch (laser)
	{
		case LASER1:
			return laser1->numvalues;
			break;
		case LASER2:
			return laser2->numvalues;
			break;
		case LASER3:
			return laser3->numvalues;
			break;
		case LASER4:
			return laser4->numvalues;
			break;
		case LASER5:
			return laser5->numvalues;
			break;
	}
	return 0;
}


float Laser::getLaserDistance(int laser, int angle)
{
	int numreadings;
	double *laserrange = NULL;


	switch (laser)
	{
		case LASER1:
			numreadings = laser1->numvalues;
			laserrange = laser1->range;
			laserrange[angle] /= 100;
			return laserrange[angle];
			break;
		case LASER2:
			numreadings = laser2->numvalues;
			laserrange = laser2->range;
			laserrange[angle] /= 100;
			return laserrange[angle];
			break;
		case LASER3:
			numreadings = laser3->numvalues;
			laserrange = laser3->range;
			laserrange[angle] /= 100;
			return laserrange[angle];
			break;
		case LASER4:
			numreadings = laser4->numvalues;
			laserrange = laser4->range;
			laserrange[angle] /= 100;
			return laserrange[angle];
			break;
		case LASER5:
			numreadings = laser5->numvalues;
			laserrange = laser5->range;
			laserrange[angle] /= 100;
			return laserrange[angle];
			break;
	}
	
	// this line should never be reached
	return 0.0;
}


void Laser::setDevicePort(short int laser, QString serialPort)
{
	// TODO: add support for laser 3 to 5
	switch (laser)
	{
		case LASER1:
			laserSerialPort1 = serialPort;
			break;
		case LASER2:
			laserSerialPort2 = serialPort;
			break;
	}
}


double Laser::carmen_radians_to_degrees(double theta)
{
	return (theta * 180.0 / M_PI);
}

double Laser::carmen_degrees_to_radians(double theta)
{
	return (theta * M_PI / 180.0);
}



/* fancy serial functions */
int Laser::iParity(parity_t par)
{
	if(par == N)
		return(IGNPAR);
	else
		return(INPCK);
}


int Laser::iSoftControl(int flowcontrol)
{
	if(flowcontrol)
		return(IXON);
	else
		return(IXOFF);
}


int Laser::cDataSize(int numbits)
{
	switch(numbits) {
	case 5:
		return(CS5);
		break;
	case 6:
		return(CS6);
		break;
	case 7:
		return(CS7);
		break;
	case 8:
		return(CS8);
		break;
	default:
		return(CS8);
		break;
	}
}


int Laser::cStopSize(int numbits)
{
	if(numbits == 2)
		return(CSTOPB);
	else
		return(0);
}


int Laser::cFlowControl(int flowcontrol)
{
	if(flowcontrol)
		return(CRTSCTS);
	else
		return(CLOCAL);
}


int Laser::cParity(parity_t par)
{
	if(par != N) {
		if(par == O)
		return(PARENB | PARODD);
		else
		return(PARENB);
	}
	else
		return(0);
}


int Laser::cBaudrate(int baudrate)
{
	switch(baudrate) {
	case 0:
		return(B0);
		break;
	case 300:
		return(B300);
		break;
	case 600:
		return(B600);
		break;
	case 1200:
		return(B1200);
		break;
	case 2400:
		return(B2400);
		break;
	case 4800:
		return(B4800);
		break;
	case 9600:
		return(B9600);
		break;
	case 19200:
		return(B19200);
		break;
	case 38400:
		return(B38400);
		break;
	case 57600:
		return(B57600);
		break;
	case 115200:
		return(B115200);
		break;
	#if !defined(CYGWIN) && !defined(__APPLE__)
	case 500000:
		/* to use 500k you have to change the entry of B460800 in you kernel:
		/usr/src/linux/drivers/usb/serial/ftdi_sio.h:
		ftdi_8U232AM_48MHz_b460800 = 0x0006    */
		return(B460800);
		break;
	#endif
	default:
		return(B9600);
		break;
	}
}


void Laser::sick_set_serial_params(sick_laser_p laser)
{
	struct termios  ctio;
	
	tcgetattr(laser->dev.fd, &ctio); /* save current port settings */
	ctio.c_iflag = iSoftControl(laser->dev.swf) | iParity(laser->dev.parity);
	ctio.c_oflag = 0;
	ctio.c_cflag = CREAD | cFlowControl(laser->dev.hwf || laser->dev.swf) | cParity(laser->dev.parity) | cDataSize(laser->dev.databits) | cStopSize(laser->dev.stopbits);
	ctio.c_lflag = 0;
	ctio.c_cc[VTIME] = 0;     /* inter-character timer unused */
	ctio.c_cc[VMIN] = 0;      /* blocking read until 0 chars received */
	cfsetispeed(&ctio, (speed_t)cBaudrate(laser->dev.baudrate));
	cfsetospeed(&ctio, (speed_t)cBaudrate(laser->dev.baudrate));
	tcflush(laser->dev.fd, TCIFLUSH);
	tcsetattr(laser->dev.fd, TCSANOW, &ctio);
}


int Laser::kernel_minimum_version( int a, int b, int c )
{
	struct utsname        uts;
	int                   ca, cb, cc; 
	uname(&uts);
	sscanf( uts.release, "%d.%d.%d", &ca, &cb, &cc );
	if (ca*65536+cb*256+cc>=a*65536+b*256+c) {
		return(TRUE);
	} else {
		return(FALSE);
	}
}


void Laser::sick_set_baudrate(sick_laser_p laser, int brate)
{
	struct termios  ctio;
	
	#if !defined(CYGWIN) && !defined(__APPLE__)
	
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20)
	struct serial_struct  serinfo;
	#endif
	
	tcgetattr(laser->dev.fd, &ctio); /* save current port settings */
	
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,20)
	if (brate==500000 && kernel_minimum_version(2,4,20)) {
		cfsetispeed ( &ctio, (speed_t) cBaudrate(38400) );
		cfsetospeed ( &ctio, (speed_t) cBaudrate(38400) );
		serinfo.reserved_char[0] = 0;
		if (ioctl(laser->dev.fd, TIOCGSERIAL, &serinfo) < 0) {
		fprintf(stderr," cannot get serial info\n");
		close(laser->dev.fd);
		// Markus shutdown_laser(1);
		}
		serinfo.flags =
		( serinfo.flags & ~ASYNC_SPD_MASK) | ASYNC_SPD_CUST;
		serinfo.custom_divisor =  serinfo.baud_base / brate;
		if (ioctl(laser->dev.fd, TIOCSSERIAL, &serinfo) < 0) {
		fprintf(stderr," cannot set serial info\n");
		close(laser->dev.fd);
		// Markus shutdown_laser(1);
		}
	} else {
		cfsetispeed ( &ctio, (speed_t) cBaudrate(brate) );
		cfsetospeed ( &ctio, (speed_t) cBaudrate(brate) );
	}
	#else 
	cfsetispeed(&ctio, (speed_t)cBaudrate(brate));
	cfsetospeed(&ctio, (speed_t)cBaudrate(brate));
	#endif
	
	#else
	
	cfsetispeed(&ctio, (speed_t)cBaudrate(brate));
	cfsetospeed(&ctio, (speed_t)cBaudrate(brate));
	
	#endif
	
	tcflush(laser->dev.fd, TCIFLUSH);
	tcsetattr(laser->dev.fd, TCSANOW, &ctio);
}


int Laser::sick_serial_connect(sick_laser_p laser)
{
	if((laser->dev.fd = open(laser->dev.ttyport, O_RDWR | O_NOCTTY, 0)) < 0)
	{
		return(-1);
	}
	
	#ifdef CARMEN_LASER_LOW_LATENCY
	serialPort->setLowLatency(laser->dev.fd);
	#endif
	
	sick_set_serial_params(laser);
	
	return(laser->dev.fd);
}


int Laser::sick_compute_checksum(unsigned char *CommData, int uLen)
{
	unsigned char abData[2] = {0, 0}, uCrc16[2] = {0, 0};
	
	while(uLen--) {
		abData[0] = abData[1];
		abData[1] = *CommData++;
		if(uCrc16[0] & 0x80) {
		uCrc16[0] <<= 1;
		if(uCrc16[1] & 0x80)
			uCrc16[0] |= 0x01;
		uCrc16[1] <<= 1;
		uCrc16[0] ^= CRC16_GEN_POL0;
		uCrc16[1] ^= CRC16_GEN_POL1;
		} 
		else {
		uCrc16[0] <<= 1;
		if(uCrc16[1] & 0x80)
			uCrc16[0] |= 0x01;
		uCrc16[1] <<= 1;
		}
		uCrc16[0] ^= abData[0];
		uCrc16[1] ^= abData[1];
	}
	return (((int)uCrc16[0]) * 256 + ((int)uCrc16[1]));
}


int Laser::sick_read_data(sick_laser_p laser, unsigned char *data, double timeout)
{
	static int val, i, j, l, pos, chk1, chk2;
	double start_time;

	#ifdef CARMEN_LASER_USE_SELECT
	fd_set read_set;
	struct timeval timer;
	timer.tv_sec=(long)(floor(timeout));
	timer.tv_usec=(long)((timeout-floor(timeout))*1000000);
	FD_ZERO(&read_set);
	FD_SET(laser->dev.fd, &read_set);
	#endif

	l    = BUFFER_SIZE;
	pos  = 0;
	chk1 = FALSE;
	chk2 = FALSE;

	#ifdef CARMEN_LASER_USE_SELECT
	while(select(laser->dev.fd+1, &read_set, NULL, NULL, &timer))
	{
		start_time = carmen_get_time();
	#else

	start_time = carmen_get_time();
		
	while(carmen_get_time() - start_time < timeout)
	{
	#endif
		val = serialPort->numChars(laser->dev.fd);
		
		if(val > 0)
		{
			if(pos + val >= BUFFER_SIZE)
				return(0);
			
			if(pos + val >= l + 6)
				val = l + 6 - pos;
	
			read(laser->dev.fd, &(data[pos]), val);
			pos += val;
			
			if(!chk1 && pos > 2)
			{
				if(data[0] != STX || data[1] != LID)
				{
					for(i = 1; i < pos - 1; i++)
					{
						if(data[i] == STX && data[i+1] == LID)
						{
							for(j = i; j < pos; j++)
							{
								data[j - i] = data[j];
							}
							pos -= i;
							chk1 = TRUE;
							break;
						}
					}
					
					if(!chk1)
						pos = 0;
				}
				else
					chk1 = TRUE;
			}
			
			if(!chk2 && pos > 4)
			{
				l = data[3] * 256 + data[2];
				chk2 = TRUE;
			}
		}
		
		if(pos == l + 6)
			return(l + 6);
		
		#ifndef CARMEN_LASER_USE_SELECT
		usleep(1000);
		#endif
	}
	
	return(0);
}


int Laser::sick_write_command(sick_laser_p laser, unsigned char command, unsigned char *argument, int arg_length)
{
	unsigned char buffer[MAX_COMMAND_SIZE];
	int pos = 0, i, check, length, loop, answer = 0, counter = 0;
	int val = 0;
	#ifdef CARMEN_LASER_USE_SELECT
	fd_set read_set;
	struct timeval timer;
	timer.tv_sec=1;
	timer.tv_usec=0;//1000*MAX_TIME_FOR_RESPONDING;
	FD_ZERO(&read_set);
	FD_SET(laser->dev.fd, &read_set);
	#endif
	
	/* SYNC CHARS */
	buffer[pos++] = 0x02;
	/* ADDRESS    */
	buffer[pos++] = 0x00;
	/* MESSAGE LENGTH */
	length = 1 + arg_length;
	buffer[pos++] = length & 0x00ff;
	buffer[pos++] = length / 256;	/* I wonder if that works */
	/* COMMAND */
	buffer[pos++] = command;
	/* ARGUMENT */
	if(arg_length > 0)
		for(i=0; i < arg_length; i++)
		buffer[pos++] = argument[i];
	/* CHECKSUM */
	check = sick_compute_checksum(buffer, length + 4);
	buffer[pos++] = check & 0x00ff;
	buffer[pos++] = check / 256;
	serialPort->writePort(laser->dev.fd, buffer, pos);
	
	/* wait for acknowledgement */
	loop = 1;
	answer = INI;
	#ifdef CARMEN_LASER_USE_SELECT
	loop=select(laser->dev.fd+1, &read_set, NULL, NULL, &timer);
	if(loop) {
	#else
	while(loop) {
	#endif
		counter++;
		val = serialPort->numChars(laser->dev.fd);
		if(val > 0) {
		read(laser->dev.fd, &buffer, val);
		switch(buffer[0]) {
		case 0x02:
		answer = STX;
		break;
		case 0x06:
		answer = ACK;
		break;
		case 0x10:
		answer = DLE;
		break;
		case 0x15:
		answer = NAK;
		break;
		default:
		answer = UKN;
		}
		loop = 0;
		} 
		else if(counter > 5) {
		answer = TIO;
		loop = 0;
		}
	#ifndef CARMEN_LASER_USE_SELECT
		usleep(1000); 
	#endif
	}
	return answer;
}


void Laser::sick_request_status(sick_laser_p laser)
{
	sick_write_command(laser, 0x31, NULL, 0);
}

void Laser::sick_request_sensor(sick_laser_p laser)
{
	static unsigned char args[2] = {0x05, 0xb4};
	
	sick_write_command(laser, 0x30, args, 2);
}


int Laser::sick_set_laser_baudrate(sick_laser_p laser, int brate)
{
	unsigned char args[1];
	int result;
	
	if(brate == 500000)
		args[0] = 0x48;
	else if(brate == 38400)
		args[0] = 0x40;
	else if(brate == 19200)
		args[0] = 0x41;
	else
		args[0] = 0x42;
	result = sick_write_command(laser, 0x20, args, 1);
	return (result == ACK);
}


int Laser::sick_set_config_mode(sick_laser_p laser)
{
	unsigned char data[MAX_COMMAND_SIZE], args[MAX_COMMAND_SIZE];
	int i, result;
	
	for(i = 0; i < 8; i++)
		args[i + 1] = (unsigned char)laser->dev.passwd[i];
	args[0] = 0x00;
	serialPort->clearInputBuffer(laser->dev.fd);
	result = sick_write_command(laser, 0x20, args, 9);
	if(result == ACK)
		return(sick_read_data(laser, data, MAX_TIME_FOR_CONFIG));
	else
		return(FALSE);
}


int Laser::sick_set_lms_resolution(sick_laser_p laser)
{
	unsigned char args[4];
	int result;
	
	if(laser->settings.angle_range == 100) {
		args[0] = 0x64; args[1] = 0x00;
		if(laser->settings.angle_resolution == RES_1_00_DEGREE) {
		args[2] = 0x64; args[3] = 0x00;
		} 
		else if(laser->settings.angle_resolution == RES_0_50_DEGREE) {
		args[2] = 0x32; args[3] = 0x00;
		} 
		else {
		args[2] = 0x19; args[3] = 0x00;
		}
	} 
	else {
		args[0] = 0xB4; args[1] = 0x00;
		if(laser->settings.angle_resolution == RES_1_00_DEGREE) {
		args[2] = 0x64; args[3] = 0x00;
		} else {
		args[2] = 0x32; args[3] = 0x00;
		}
	}
	result = sick_write_command(laser, 0x3B, args, 4);
	return(result == ACK);
}


int Laser::sick_request_lms_config(sick_laser_p laser)
{
	int result;
	
	result = sick_write_command(laser, 0x74, NULL, 0);
	return(result == ACK);
}


int Laser::sick_set_lms_config(sick_laser_p laser, unsigned char *data, int len)
{
	int result;
	
	if(len == 32) {
		result = sick_write_command(laser, 0x77, data, len);
		return(result == ACK);
	} 
	else
		return(FALSE);
}


int Laser::sick_parse_conf_data(sick_laser_p laser, unsigned char *buf, int length)
{
	int check, i;
	unsigned char data[32];
	
	if(length < 4)
		return(FALSE);
	else
		check = sick_compute_checksum(buf, length - 2);
	if((length != 42 && length != 40) || buf[4] != 0xF4 ||
		buf[length - 2] != (check & 0x00ff) || buf[length - 1] != (check / 256))
		return(FALSE);
	for(i = 0; i < 32; i++)
		data[i] = buf[i + 5];
	if((laser->settings.range_res == CM && data[6] != 0) ||
		(laser->settings.range_res == MM && data[6] != 1) ||
		(laser->settings.range_res == DM && data[6] != 2) ||
		(laser->settings.range_dist == SICK_RANGE80M  && data[5] != 1) ||
		(laser->settings.range_dist == SICK_RANGE160M && data[5] != 3) ||
		(laser->settings.range_dist == SICK_RANGE320M && data[5] != 5) ||
		(laser->settings.range_dist == SICK_REMISSION_NORM && data[5] != 13) ||
		(laser->settings.range_dist == SICK_REMISSION_DIRECT && data[5] != 14)) {
	
		//    fprintf(stderr, "ok\n");
		fprintf(stderr, "config-mode ... ");
		do {} while (!sick_set_config_mode(laser));
		fprintf(stderr, "ok ...");
	
		/* fix the laser sunlight problem */
		//    data[4] |= 1;
	
		switch(laser->settings.range_dist) {
		case SICK_RANGE80M:
		data[5] = 1; 
		break;
		case SICK_RANGE160M:
		data[5] = 3;
		break;
		case SICK_RANGE320M:
		data[5] = 5;
		break;
		case SICK_REMISSION_NORM:
		data[5] = 13;
		break;
		case SICK_REMISSION_DIRECT:
		data[5] = 14;
		break;
		default:
		data[5] = 1;
		break;
		}
		switch(laser->settings.range_res) {
		case CM:
		data[6] = 0;
		break;
		case MM:
		data[6] = 1;
		break;
		case DM:
		data[6] = 2;
		break;
		default:
		data[6] = 0;
		break;
		}
		fprintf(stderr, " set LMS config ... ");
		do {} while (!sick_set_lms_config(laser, data, 32));
		fprintf(stderr, "ok ...");
	}
	return(TRUE);
}


int Laser::sick_set_lms_range(sick_laser_p laser)
{
	int l = 0;
	unsigned char data[BUFFER_SIZE];
	
	if(sick_request_lms_config(laser)) {
		usleep(100000);
		l = sick_read_data(laser, data, MAX_TIME_FOR_GETTING_CONF);
		return(sick_parse_conf_data(laser, data, l));
	} 
	else
		return(FALSE);
}


void Laser::sick_start_continuous_mode(sick_laser_p laser)
{
	unsigned char lmsarg[1] = {0x24}, pls180arg[1] = {0x20};
	unsigned char pls360arg[1] = {0x24};
	int result = 0;
	
	do {
		if(laser->settings.type == LMS)
		result = sick_write_command(laser, 0x20, lmsarg, 1);
		else if(laser->settings.type == PLS &&
			laser->settings.angle_resolution == RES_1_00_DEGREE)
		result = sick_write_command(laser, 0x20, pls180arg, 1);
		else if(laser->settings.type == PLS &&
			laser->settings.angle_resolution == RES_0_50_DEGREE)
		result = sick_write_command(laser, 0x20, pls360arg, 1);
	} while(result != ACK);
}


void Laser::sick_stop_continuous_mode(sick_laser_p laser)
{
	unsigned char args[1] = {0x25};
	int result;
	
	do {
		result = sick_write_command(laser, 0x20, args, 1);
	} while(result != ACK);
}


void Laser::sick_start_continuous_remission_part_mode(sick_laser_p laser)
{
	unsigned char lmsarg[7] = {0x2b, 1,0,0,0,0,0}, pls180arg[1] = {0x20};
	unsigned char pls360arg[1] = {0x24};
	int result = 0;
	
	if(laser->settings.type == LMS) {
		lmsarg[3] = 1;
		lmsarg[5] = laser->settings.rem_values;
	}
	else if(laser->settings.type == PLS) {
		laser->settings.use_remission = 0;
		fprintf(stderr, "WARNING: No remission-mode support for non LMS lasers. Continuing normal mode.\n"); 
	}
	
	do {
		if(laser->settings.type == LMS) 
		result = sick_write_command(laser, 0x20, lmsarg, 7);
		else if(laser->settings.type == PLS &&
			laser->settings.angle_resolution == RES_1_00_DEGREE)
		result = sick_write_command(laser, 0x20, pls180arg, 1);
		else if(laser->settings.type == PLS &&
			laser->settings.angle_resolution == RES_0_50_DEGREE)
		result = sick_write_command(laser, 0x20, pls360arg, 1);
	} while(result != ACK);
}


int Laser::sick_testBaudrate(sick_laser_p laser, int brate)
{
	unsigned char data[BUFFER_SIZE], ReqLaser[2] = {5, 180};
	int response;
	
	// start Markus: time measure added
	//	double start_time = 0;
	//	double elapsed_time = 0;
	
	// get time
	//	start_time = carmen_get_time();
	//qDebug("\nstart time: %f seks", start_time);
	// end Markus


	sick_set_baudrate(laser, brate);
	//	elapsed_time = (carmen_get_time() - start_time);
	//	qDebug("elapsed time till here: %f seks", elapsed_time);
	
	response = sick_write_command(laser, 0x30, ReqLaser, 2);
	if(response == NAK)
	{
		fprintf(stderr, "Error : Could not send command to laser.\n");
		return FALSE;
	}
	
	if(sick_read_data(laser, data, MAX_TIME_FOR_TESTING_BAUDRATE))
	{
		return TRUE;
	}
	
	return FALSE;
}


int Laser::sick_detect_baudrate(sick_laser_p laser)
{
	emit(message(QString("Connecting to Laser Scanner %1 with 9600 bps...").arg(laser->settings.device_name)));
	if(sick_testBaudrate(laser, 9600))
	{
		return(9600);
	}
	else
	{
		emit(message(QString("Connecting to Laser Scanner %1 with 19200 bps...").arg(laser->settings.device_name)));
		if(sick_testBaudrate(laser, 19200))
		{
			return(19200);
		}
		else
		{
			emit(message(QString("Connecting to Laser Scanner %1 with 38400 bps...").arg(laser->settings.device_name)));
			if(sick_testBaudrate(laser, 38400))
			{
				return(38400);
			}
			else
			{
				if(laser->settings.use_highspeed)
				{
					emit(message(QString("Connecting to Laser Scanner %1 with 500000 bps...").arg(laser->settings.device_name)));
					if(sick_testBaudrate(laser, 500000))
					{
						return(500000);
					}
					else
					{
						emit(message(QString("Conntection to Laser Scanner %1 FAILED!").arg(laser->settings.device_name)));
						return(0);
					}
				}
				else
				{
					emit(message(QString("Conntection to Laser Scanner %1 FAILED!").arg(laser->settings.device_name)));
					return(0);
				}
			}
		}
	}
}


int Laser::sick_check_baudrate(sick_laser_p laser, int brate)
{
	fprintf(stderr, "check baudrate:\n");
	fprintf(stderr, "    %d ... ", brate);
	if(sick_testBaudrate(laser, brate)) {
		fprintf(stderr, "yes\n");
		return(TRUE);
	} 
	else {
		fprintf(stderr, "no\n");
		return(FALSE);
	}
}


void Laser::sick_install_settings(sick_laser_p laser)
{
	laser->dev.type = laser->settings.type;
	laser->dev.baudrate = laser->settings.start_baudrate;
	laser->dev.parity = laser->settings.parity;
	laser->dev.fd = -1;
	laser->dev.databits = laser->settings.databits;
	laser->dev.stopbits = laser->settings.stopbits;
	laser->dev.hwf = laser->settings.hwf;
	laser->dev.swf = laser->settings.swf;
	strncpy((char *)laser->dev.passwd, (const char *)laser->settings.password, 8);
	laser->dev.ttyport = (char *)malloc((strlen(laser->settings.device_name) + 1) * sizeof(char));

	// not nedded:
	// carmen_test_alloc(laser->dev.ttyport);

	strcpy(laser->dev.ttyport, laser->settings.device_name);
}


void Laser::sick_allocate_laser(sick_laser_p laser)
{
	int i;
	
	laser->numvalues = laser->settings.num_values;
	
	laser->range = (double *)malloc(laser->settings.num_values * sizeof(double));
	// Not nedded:
	//carmen_test_alloc(laser->range);
	
	laser->glare = (int *)malloc(laser->settings.num_values * sizeof(int));
	// Not nedded:
	// carmen_test_alloc(laser->glare);
	
	laser->wfv = (int *)malloc(laser->settings.num_values * sizeof(int));
	// Not nedded:
	//carmen_test_alloc(laser->wfv);
	
	laser->sfv = (int *)malloc(laser->settings.num_values * sizeof(int));
	// Not nedded:
	//carmen_test_alloc(laser->sfv);
	
	laser->buffer = (unsigned char *)malloc(LASER_BUFFER_SIZE);
	// Not nedded:
	//carmen_test_alloc(laser->buffer);
	
	laser->new_reading = 0;
	laser->buffer_position = 0;
	laser->processed_mark = 0;
	
	for(i = 0; i < laser->settings.num_values; i++)
	{
		laser->range[i] = 0.0;
		laser->glare[i] = 0;
		laser->wfv[i] = 0;
		laser->sfv[i] = 0;
	}
	
	if(laser->settings.use_remission)
	{
		laser->remvalues = laser->settings.rem_values;
		laser->remission = (double *)malloc(laser->settings.rem_values * sizeof(double));
		// not needed here:
		//carmen_test_alloc(laser->remission);
		
		for(i = 0; i< laser->settings.rem_values; i++)
	  		laser->remission[i] = 0;
	}
}


int Laser::sick_connect_device(sick_laser_p laser)
{
	sick_install_settings(laser);
	sick_allocate_laser(laser);
	
	emit(message(QString("Connecting Laser Scanner to %1...").arg(laser->dev.ttyport)));
	sick_serial_connect(laser);
	
	if(laser->dev.fd == -1)
	{
		emit(message(QString("Connecting Laser Scanner to %1...failed!").arg(laser->dev.ttyport)));
		return 1;
	}
	
	emit(message(QString("Setting Laser Scanner port parameters to %1/%2/%3/%4...").arg(laser->dev.baudrate).arg(laser->dev.databits).arg(laser->dev.parity).arg(laser->dev.stopbits)));
	sick_set_serial_params(laser);
	
	return 0;
}


int Laser::sick_start_laser(sick_laser_p laser)
{
	int brate = 0;


	/* open the serial port */
	if (sick_connect_device(laser) != 0)
	{
		return 1;
	}


	/* make sure the baudrate is set correctly, change it if necessary */
	if(laser->settings.detect_baudrate)
	{
		brate = sick_detect_baudrate(laser);
		
		if (!brate)
		{
			return 1;
			//exit(1);
		}
	}
	else if(!sick_check_baudrate(laser, laser->settings.start_baudrate))
	{
		fprintf(stderr, "ERROR: communication does not work!\n");
		return 1;
		//exit(1);
	}


	if(brate != laser->settings.set_baudrate)
	{
		emit(message("Setting Laser Scanner in config mode..."));
		while(!sick_set_config_mode(laser))
		{
		};
		
		emit(message(QString("Setting Laser Scanner baudrate to %1...").arg(laser->settings.set_baudrate)));
		
		while(!sick_set_laser_baudrate(laser, laser->settings.set_baudrate))
		{
		};
		
		sick_set_baudrate(laser, laser->settings.set_baudrate);
	}

	
	/* set the resolution of the blue lasers */
	if(laser->settings.type == LMS)
	{
		emit(message(QString("Setting Laser Scanner to resolution %1...").arg(laser->settings.angle_resolution)));
		while(!sick_set_lms_resolution(laser))
		{
		};
		
		usleep(100000);
		
		emit(message(QString("Setting Laser Scanner to range %1...").arg(laser->settings.angle_range)));
		while(!sick_set_lms_range(laser))
		{
		};
	}


	/* set the laser to continuous mode */
	if(laser->settings.use_remission == 1)
	{
		emit(message("Setting Laser Scanner to continuous remission mode..."));
		sick_start_continuous_remission_part_mode(laser);
	}
	else
	{
		emit(message("Setting Laser Scanner to continuous mode..."));
		sick_start_continuous_mode(laser);
	}

	laser->packet_timestamp=-1;
	
	return 0;
}


int Laser::sick_valid_packet(unsigned char *data, long size, long *offset, long *len)
{
	int i, check, packet_size = 0, theo_size = 0;
	
	for(i = 0; i < size; i++) {
		if(packet_size == 0 && *data == 0x02)
		packet_size++;
		else if(packet_size == 1 && *data == 0x80)
		packet_size++;
		else if(packet_size == 1)
		packet_size = 0;
		else if(packet_size == 2) {
		theo_size = data[0];
		packet_size++;
		} 
		else if(packet_size == 3) {
		theo_size += (data[0] << 8) + 6;
		if(size >= theo_size + (i - packet_size)) {	// check the crc
		check = data[theo_size - 3 - 2];
		check += data[theo_size - 3 - 1] << 8;
		if(check != sick_compute_checksum(data - 3, theo_size - 2)) {
		i -= 2;
		data -= 2;
		packet_size = 0;
		}
		else {
		*offset = i - packet_size;
		*len = theo_size;
		return 1;
		}
		} 
		else
		packet_size = 0;
		}
		data++;
	}
	return 0;
}


void Laser::sick_process_packet_distance(sick_laser_p laser, unsigned char *packet)
{
	int i = 0, LoB = 0, HiB = 0, bit14, bit15, numMeasurements;
	float conversion = 1.0;
	
	if(packet[0] == 0xb0)
	{
		/* Check the number of measurements */
		numMeasurements = ((packet[2] << 8) + packet[1]) & 0x3FFF;
	
		/* Extract the distance conversion factor */
		bit14 = packet[2] & 0x40;
		bit15 = packet[2] & 0x80;
		if(laser->settings.type == LMS)
		{
			if(!bit15)
			if(!bit14)
			conversion = 1.0;
			else
			conversion = 0.1;
			else
			conversion = 10.0;
		}
		
		/* Compute range values */
		laser->new_reading = 1;
		for (i = 0; i < numMeasurements; i++)
		{
			LoB = packet[i * 2 + 3]; 
			HiB = packet[i * 2 + 4];
			laser->range[i] = ((HiB & 0x1f) * 256 + LoB) * conversion;
			laser->glare[i] = (HiB & 0x20) / 32; 
			laser->wfv[i] = (HiB & 0x40) / 64;  
			laser->sfv[i] = (HiB & 0x80) / 128;
		}
	}
}


void Laser::sick_process_packet_remission(sick_laser_p laser, unsigned char *packet)
{
	int i = 0, LoB = 0, HiB = 0, bit14, bit15, numMeasurements;
	int parts, mstart, mend;
	int offs;
	float conversion = 1.0;
	
	if(packet[0] == 0xf5)
	{
		/* Extract number of scan parts (should be 1) */
		parts = packet[1] & 0x7;
		
		offs = 0;
	
		mstart = ((packet[offs + 4] << 8) + packet[offs + 3]);
		mend   = ((packet[offs + 6] << 8) + packet[offs + 5]);
		//  fprintf(stderr, "mstart, mend = %d, %d\n", mstart, mend);
	
		/* Check the number of measurements */
		numMeasurements = ((packet[offs + 8] << 8) + packet[offs + 7]) & 0x3FFF;
		//  fprintf(stderr, "num_measurem. = %d\n",numMeasurements);
	
		/* Extract the distance conversion factor */
		bit14 = packet[offs + 8] & 0x40;
		bit15 = packet[offs + 8] & 0x80;
		if(laser->settings.type == LMS)
		{
			if(!bit15)
				if(!bit14)
					conversion = 1.0;
				else
					conversion = 0.1;
				else
					conversion = 10.0;
		}
		
		/* Compute range values */
		laser->new_reading = 1;
		for (i = 0; i < numMeasurements; i++)
		{
			LoB = packet[i * 4 + offs + 9]; 
			HiB = packet[i * 4 + offs + 10];
			laser->range[i] = (HiB * 256 + LoB) * conversion;
			laser->remission[i] = packet[i * 4 + offs + 12] * 256 + packet[i * 4 + offs + 11];
		}
	}
}


void Laser::sick_process_packet(sick_laser_p laser, unsigned char *packet)
{
	if(laser->settings.use_remission == 1)
		sick_process_packet_remission(laser, packet);
	else sick_process_packet_distance(laser, packet);
}


void Laser::sick_handle_laser(sick_laser_p laser)
{
	int bytes_available, bytes_read;
	int leftover;
	
	laser->new_reading = 0;
	#ifdef CARMEN_LASER_USE_SELECT
	double timeout=0.1;
	fd_set read_set;
	struct timeval timer;
	timer.tv_sec=(long)(floor(timeout));
	timer.tv_usec=(long)((timeout-floor(timeout))*1000000);
	FD_ZERO(&read_set);
	FD_SET(laser->dev.fd, &read_set);
	select(laser->dev.fd+1, &read_set, NULL, NULL, &timer);
	#endif
	/* read what is available in the buffer */
	bytes_available = serialPort->numChars(laser->dev.fd);
	
	if (bytes_available > LASER_BUFFER_SIZE - laser->buffer_position)
	{
		bytes_available = LASER_BUFFER_SIZE - laser->buffer_position;
	}
	
	bytes_read = serialPort->readPort(laser->dev.fd, laser->buffer + laser->buffer_position, bytes_available);
	
	/* process at most one laser reading */
	if(bytes_read > 0)
	{
		if (laser->packet_timestamp<0.)
		{
			laser->packet_timestamp=carmen_get_time();
		}
		
		laser->buffer_position += bytes_read;
		
		if(sick_valid_packet(laser->buffer + laser->processed_mark, laser->buffer_position - laser->processed_mark, &(laser->packet_offset), &(laser->packet_length)))
		{
			laser->timestamp=laser->packet_timestamp;
			sick_process_packet(laser, laser->buffer + laser->processed_mark + laser->packet_offset + 4);
			laser->packet_timestamp=-1.;
	
			leftover = laser->buffer_position - laser->processed_mark - laser->packet_length;
			laser->processed_mark += laser->packet_offset + laser->packet_length;
		
			//PACKET_DROPPING
			while (leftover>laser->packet_length)
			{
				laser->processed_mark +=laser->packet_length;
				leftover-=laser->packet_length;
				// TODO: change std output
				// fprintf(stderr,"D");
			}
		
			if(leftover == 0)
			{
				laser->buffer_position = 0;
				laser->processed_mark = 0;
			}
		}
	}
	
	
	/* shift everything forward in the buffer, if necessary */
	if(laser->buffer_position > LASER_BUFFER_SIZE / 2)
	{
		memmove(laser->buffer, laser->buffer + laser->processed_mark, laser->buffer_position - laser->processed_mark);
		laser->buffer_position = laser->buffer_position - laser->processed_mark;
		laser->processed_mark = 0;
	}
}


void Laser::sick_stop_laser(sick_laser_p laser)
{
	emit(message(QString("Stopping Laser Scanner continuous mode for Laser Scanner %1...").arg(laser->settings.device_name)));
	sick_stop_continuous_mode(laser);
	emit(message(QString("Stopping Laser Scanner continuous mode for Laser Scanner %1...OK.").arg(laser->settings.device_name)));
	close(laser->dev.fd);
}


double Laser::carmen_get_time(void)
{
	struct timeval tv;
	double t;

	if (gettimeofday(&tv, NULL) < 0)
	{
		qDebug("carmen_get_time encountered error in gettimeofday : %s", strerror(errno));
	}
	
	t = tv.tv_sec + tv.tv_usec/1000000.0;
	
	return t;
}
