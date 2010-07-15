/*************************************************************************
 *   Copyright (C) 2010 by Markus Knapp                                  *
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

#include <stdio.h>
#import <Cocoa/Cocoa.h>
#import <Foundation/NSPathUtilities.h> /* -framework Foundation */

FILE *JoyReaderOpenJoystickCalibrationFileC(const char mode[])
{
	FILE *fp;

	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	NSString *homeDir;
	NSString *cwd;

	homeDir=NSHomeDirectory();  // Do I release the string?
	cwd=[[NSFileManager defaultManager] currentDirectoryPath];

	[[NSFileManager defaultManager] changeCurrentDirectoryPath:homeDir];

	fp=fopen(".ysjoycalib",mode);

	[[NSFileManager defaultManager] changeCurrentDirectoryPath:cwd];

	[pool release];

	return fp;
}
