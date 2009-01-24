#include "init.h"


int main(int argc, char *argv[])
{
//	unsigned char c = 64; // INIT
	unsigned char c = 40; // flashlight off
//	unsigned char c = 41; // flashlight on
	
	
	printf("\nOpening serial port...");
	int dev_fd = open_port();
	printf("Opened.\n");

	printf("Configuring serial port...");
	configure_port(dev_fd);
	printf("Configured.\n");
	
	printf("Sending command...");
	write_port(dev_fd, &c, 1);
	printf("Sent.\n");
	
	printf("Closing serial port...");
	close_port(dev_fd);
	printf("Closed.\n\n");
	
	return 0;
}


int open_port(void)
{
	/*
	Returns the file descriptor on success or -1 on error.
	*/
	int fd; /* File descriptor for the port */


	/*
	The O_NOCTTY flag tells UNIX that this program doesn't want to be the "controlling terminal" for that port.
	If you don't specify this then any input (such as keyboard abort signals and so forth) will affect your process.
	Programs like getty(1M/8) use this feature when starting the login process, but normally a user program does not want this behavior.
	
	The O_NDELAY flag tells UNIX that this program doesn't care what state the DCD signal line is in - whether the other end of the port is up and running.
	If you do not specify this flag, your process will be put to sleep until the DCD signal line is the space voltage.
	*/
	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	
	if (fd == -1)
	{
		/*
		* Could not open the port.
		*/
		printf("open_port: Unable to open /dev/ttyUSB0\n");
	}
	else
	{
		fcntl(fd, F_SETFL, 0);
	}

	return (fd);
}


void configure_port(int dev_fd)
{
	struct termios options;
	
	/*
	* GET the current options for the port...
	*/
	tcgetattr(dev_fd, &options);
	
	/*
	* Setting the baud rate.
	*/
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	
	/*
	* Enable the receiver and set local mode...
	*/
	options.c_cflag |= (CLOCAL | CREAD);

	/*
	* Setting the Character Size
	*/
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
	options.c_cflag |= CS8;    /* Select 8 data bits */

	/*
	* Setting Parity Checking
	*/
	// No parity (8N1)
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	/*
	* Disabling Hardware Flow Control (Also called CRTSCTS)
	*/
	options.c_cflag &= ~CRTSCTS;
	
	/*
	* SET the new options for the port...
	*/
	tcsetattr(dev_fd, TCSANOW, &options);

	// FLUSH
	fcntl(dev_fd, F_SETFL, 0);
}


int read_port(int dev_fd, unsigned char *buf, int nChars)
{
	int amountRead = 0, bytes_read = 0;
	/*
	Reading Data from the Port
	Reading data from a port is a little trickier.
	When you operate the port in raw data mode, each read system call will return the number of characters that are actually available in the serial input buffers.
	If no characters are available, the call will block (wait) until characters come in, an interval timer expires, or an error occurs.
	The read function can be made to return immediately by doing the following.
	*/
	//fcntl(dev_fd, F_SETFL, FNDELAY);

	/*
	The FNDELAY option causes the read function to return 0 if no characters are available on the port.
	To restore normal (blocking) behavior, call fcntl() without the FNDELAY option.
	This is also used after opening a serial port with the O_NDELAY option.
	*/
	fcntl(dev_fd, F_SETFL, 0);
	
	// read!
	amountRead = read(dev_fd, buf, nChars);
}


int write_port(int dev_fd, unsigned char *c, int nChars)
{
	/*
	Writing Data to the Port
	Writing data to the port is easy - just use the write(2) system call to send data it.
	The write function returns the number of bytes sent or -1 if an error occurred.
	Usually the only error you'll run into is EIO when a MODEM or data link drops the Data Carrier Detect (DCD) line.
	This condition will persist until you close the port.int write_port(void)
	*/
	int n = write(dev_fd, c, 1);
	
	if (n < 0)
	{
		// error
		fputs("write() of n bytes failed!\n", stderr);
	}
	else
	{
		printf("%d byte(s) written.\n", n);
	}
	
	return n;
}


void close_port(int dev_fd)
{
	/*
	To close the serial port, just use the close system call.
	*/
	close(dev_fd);
}
