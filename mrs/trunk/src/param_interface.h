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

// Markus:
/* * @addtogroup param_daemon libparam_interface * */
// @{

/** \file param_interface.h
 * \brief Definition of the interface of the module param_daemon.
 *
 * This file specifies the interface to subscribe the messages of
 * that module and to receive its data via ipc.
 **/

#ifndef CARMEN_PARAMETER_INTERFACE_H
#define CARMEN_PARAMETER_INTERFACE_H

//#include "param_messages.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Markus
#ifdef __cplusplus
extern "C" {
#endif
*/
#define carmen_param_handle_error(error, usage, progname) {if ((error) < 0) usage(progname, carmen_param_get_error());}

#define CARMEN_PARAM_INT      1
#define CARMEN_PARAM_DOUBLE   2
#define CARMEN_PARAM_ONOFF    3
#define CARMEN_PARAM_STRING   4
#define CARMEN_PARAM_FILE     5
#define CARMEN_PARAM_DIR      6

#define CARMEN_PARAM_EXPERT  64

typedef char carmen_param_type_t;

typedef void (*carmen_param_change_handler_t)(char *module, char *variable, char *value);

  /** This data structure is used by carmen_param_install_params and
      carmen_param_usage to load and report the usage of a large number of
      parameters. 
  */ 

typedef struct {
  char *module;                      /**<The module name of this parameter. */
  char *variable;                    /**<The variable name to be loaded. */
  carmen_param_type_t type;          /**<Type should match user_variable:
					 e.g., CARMEN_PARAM_INT if the local
					 variable storage is an integer. */
  void *user_variable;               /**<A pointer to the local variable
					storage. */
  int subscribe;                     /**<If the param_daemon publishes a
					change to this variable value (because
					someone has changed the variable
					value, should the local value be
					updated? 1 for yes, 0 for no. */
  carmen_param_change_handler_t handler; /**<Declare a handler if the
					    param_daemon publishes a change to
					    this variable's value. */
} carmen_param_t, *carmen_param_p;

  /** Returns what robot (i.e., what parameter set) has been loaded into the
      param_daemon. 
  */

  char *carmen_param_get_robot(void);

  /** Returns a complete list of module names (as determined from
      the ini file by the set of variables with a prepended module name. 
  */

  int carmen_param_get_modules(char ***modules, int *num_modules);

  /** libparam_interface.a will keep a persistent module name, for
      getting/setting multiple variables of the same module. Probably should
      be deprecated. This function will change what module the library is
      currently dealing with.  
  */

  void carmen_param_set_module(char *new_module_name);

  /** libparam_interface.a will keep a persistent module name, for
      getting/setting multiple variables of the same module. Probably should
      be deprecated. This function will return what module the library is
      currently dealing with. 
  
  */      
      
  char *carmen_param_get_module(void);
  
  /** Returns the hostname where param_daemon is running. Should be changed to
      carmen_param_get_paramdaemon_host. 
  */ 
  
  int carmen_param_get_paramserver_host(char **hostname);

  /** Returns a list of all variables and their values for a specific
      module. 
  */ 

  int carmen_param_get_all(char *module, char ***variables, char ***values, 
			   int **expert, int *list_length);

  int carmen_param_get_int(char *variable, int *return_value, int *expert);
  int carmen_param_get_double(char *variable, double *return_value, 
			      int *expert);
  int carmen_param_get_onoff(char *variable, int *return_value, int *expert);
  int carmen_param_get_string(char *variable, char **return_value, 
			      int *expert);

  /** Does much the same thing s carmen_param_set_string, but checks to see if
      the returned string matches a local file. Probably should be
      deprecated. 
  */

  int carmen_param_get_filename(char *variable, char  **return_value, 
				int *expert);
  
  int carmen_param_set_variable(char *variable, char *new_value, 
			      char **return_value);
  int carmen_param_set_int(char *variable, int new_value, int *return_value);
  int carmen_param_set_double(char *variable, double  new_value, 
			      double *return_value);
  int carmen_param_set_onoff(char *variable, int new_value, int *return_value);
  int carmen_param_set_string(char *variable, char *new_value, 
			      char **return_value);

  /** Basically a wrapper around carmen_param_set_variable. 
   */

  int carmen_param_set_filename(char *variable, char *new_value,
				char **return_value);
  
  /** If an interface function recently returned an error, returns a
      human-readable string describing the error. 
  */

  char *carmen_param_get_error(void);

  /** If unfound variables are allowed, causes carmen_param_get_XXX to return
      0 even if the param_daemon does not have a definition. Otherwise,
      carmen_param_get_XXX returns -1 in such cases. 
  */

  void carmen_param_allow_unfound_variables(int new_value);
  int carmen_param_are_unfound_variables_allowed(void);
  
  void carmen_param_set_usage_line(char *fmt, ...);

  /** Allows a set of parameters to be loaded at once from a carmen_param_t
      structure. 
  */

  int carmen_param_install_params(int argc, char *argv[], 
				  carmen_param_p param_list, 
				  int num_items);
  
  /** Can be used to print a list of parameters this program loads. For
   *  example, to be called if --help is a command-line argument.
   *
   *  @param progname The name of this program (usually argv[0]). 
   *  @param param_list A set of parameters that this program will load. 
   *  @param num_items The number of parameters in the param_list. 
   *  @param Any other text that should be printed out as part of the usage
   *  string, parsed just like printf. 
   */

  void carmen_param_usage(char *progname, carmen_param_p param_list, 
			  int num_items, char *fmt, ...);
  
  void carmen_param_load_paramfile(char *filename, char *param_set);
  
  void carmen_param_check_unhandled_commandline_args(int argc, char *argv[]);
  
  void carmen_param_subscribe_int(char *module, char *variable, 
				  int *variable_address, 
				  carmen_param_change_handler_t handler);
  void carmen_param_subscribe_double(char *module, char *variable, 
				     double *variable_address, 
				     carmen_param_change_handler_t handler);
  void carmen_param_subscribe_onoff(char *module, char *variable, 
				    int *variable_address, 
				    carmen_param_change_handler_t handler);
  void carmen_param_subscribe_string(char *module, char *variable, 
				     char **variable_address, 
				     carmen_param_change_handler_t handler);
  void carmen_param_subscribe_file(char *module, char *variable, 
				   char **variable_address, 
				   carmen_param_change_handler_t handler);
  void carmen_param_subscribe_dir(char *module, char *variable, 
				  char **variable_address, 
				  carmen_param_change_handler_t handler);
  
  /** Checks to make sure the param_daemon is running the same version of
      carmen as this module. Calls exit (-1) if they don't match. 
  */
  
  int carmen_param_check_version(char *prog_name);

  /** Tells the param_daemon to re-read the ini file, and forget all local
      changes. 
  */
  
  void carmen_param_send_reread(void);
  
/*
#ifdef __cplusplus
}
#endif
*/
  
#endif
// @}
