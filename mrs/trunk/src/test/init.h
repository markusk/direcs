#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */


static const unsigned char FLASHLIGHT_OFF 	= 40;
static const unsigned char FLASHLIGHT_ON 	= 41;


int  open_port(void);
void configure_port(int dev_fd);
int  read_port(int dev_fd, unsigned char *buf, int nChars);
int  write_port(int dev_fd, unsigned char *c, int nChars);
void close_port(int dev_fd);
