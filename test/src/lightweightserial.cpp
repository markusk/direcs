///////////////////////////////////////////////////////////////////////////////
// The serial_port package provides small, simple static libraries to access 
// serial devices
//
// Copyright (C) 2008, Morgan Quigley
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright 
//     notice, this list of conditions and the following disclaimer in the 
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of Stanford University nor the names of its 
//     contributors may be used to endorse or promote products derived from 
//     this software without specific prior written permission.
//   
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#include "lightweightserial.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

LightweightSerial::LightweightSerial(const char *port, int baud) : 
  baud(baud), fd(0), happy(false)
{
	printf("about to try to open [%s]\n", port);
	fd = open(port, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0)
	{
		printf(" ahhhhhhhhh couldn't open port [%s]\n", port);
		return;
	}
	else
		printf("opened [%s] successfully\n", port);
  // put the port in nonblocking mode
	struct termios oldtio, newtio;
	if (tcgetattr(fd, &oldtio) < 0)
	{
		printf("ahhhhhhh couldn't run tcgetattr()\n");
		return;
	}
	bzero(&newtio, sizeof(newtio));
	newtio.c_iflag = IGNPAR | INPCK;
	newtio.c_oflag = 0;
	newtio.c_cflag = CS8 | CLOCAL | CREAD;
	newtio.c_lflag = 0;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 0; // poll
	cfsetspeed(&newtio, baud);
	tcflush(fd, TCIOFLUSH);
	if (tcsetattr(fd, TCSANOW, &newtio) < 0)
	{
		printf(" ahhhhhhhhhhh tcsetattr failed\n");
		return;
	}

	// flush the buffer of the serial device
	uint8_t b;
	while (this->read(&b) > 0) { }
  happy = true;
}

LightweightSerial::~LightweightSerial()
{
  if (fd > 0)
    close(fd);
  fd = 0; // prevent future reads...
}

bool LightweightSerial::read(uint8_t *b)
{
  if (!happy)
    return false;
  long nread;
	nread = ::read(fd,b,1);
	if (nread < 0)
	{
		printf("ahhhhhh read returned <0\n");
    happy = false;
		return false;
	}
	return (nread == 1);
}

int LightweightSerial::read_block(uint8_t *block, uint32_t max_read_len)
{
	if (!happy)
    return false;
  long nread = ::read(fd,block,(size_t)max_read_len);
  return (nread < 0 ? 0 : nread);
}

bool LightweightSerial::write(const uint8_t b)
{
  if (!happy)
    return false;
	if (fd >= 0 && ::write(fd, &b, 1) < 0)
    return false;
  else
    return true;
}

bool LightweightSerial::write_block(const uint8_t *block, uint32_t block_len)
{
	if (fd >= 0 && ::write(fd, block, block_len) < 0)
    return false;
  tcflush(fd, TCOFLUSH);
  return true;
}

