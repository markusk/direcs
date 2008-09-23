/***************************************************************************
 *   Copyright (C) 2008 by Markus Knapp                                    *
 *   mrs@direcs.de                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "main.h"

int main(int argc, char *argv[])
{
	//int exec;

	//QApplication app(argc, argv);
	//qDebug("Speak with Qt 4!");

	//exec = app.exec();
	//return exec;

	unsigned char c = 250; // INIT
	int amount = 0;
	unsigned char text[] = {'H','e','l','l','o',' ','M','a','r','k','u','s','@'};
	
	
	printf("\nOpening serial port...");
	int dev_fd = open_port();
	if (dev_fd == -1)
		return -1;

	printf("Opened.\n");
	
	printf("Configuring serial port...");
	configure_port(dev_fd);
	printf("Configured.\n");
	
	
	printf("Sending '0x00'...");
	c = 0x00; // end of transmit. start speaking!
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}
	printf("Sent.\n");

	
	printf("Sending commands...");

	c = 0x80; // 'speak' command
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 0x00; // full volume (0 to 7)
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 0x07; // speed pitch (0 to 7 (7 is the lowest))
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 0x02; // speed speed (0 to 7)
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}
	
	// send speech string
	printf("\n");
	c = 0;
	int n=0;
	while (c != '@')
	{
		c = text[n];
		//printf("text[%d]=%c\n", n, c);
		printf("%c", c);

		write_port(dev_fd, &c, 1);
		while ( read_port(dev_fd, &c, 1) < 1 )
		{
		}
		n++;
		c = text[n];
	}
	printf("\n");

/*
	c = 'H';
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 'e';
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 'l';
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 'l';
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	c = 'o';
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}
*/
	
	c = 0x00; // end of transmit. start speaking!
	write_port(dev_fd, &c, 1);
	while ( read_port(dev_fd, &c, 1) < 1 )
	{
	}

	printf("Sent.\n");
	
	printf("Closing serial port...");
	close_port(dev_fd);
	printf("Closed.\n\n");
}
