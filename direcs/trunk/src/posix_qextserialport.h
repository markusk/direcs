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


#ifndef _POSIX_QEXTSERIALPORT_H_
#define _POSIX_QEXTSERIALPORT_H_

#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include "qextserialbase.h"

class Posix_QextSerialPort:public QextSerialBase {
public:
    Posix_QextSerialPort();
    Posix_QextSerialPort(const Posix_QextSerialPort& s);
    Posix_QextSerialPort(const QString & name);
    Posix_QextSerialPort(const PortSettings& settings);
    Posix_QextSerialPort(const QString & name, const PortSettings& settings);
    Posix_QextSerialPort& operator=(const Posix_QextSerialPort& s);
    virtual ~Posix_QextSerialPort();

    virtual void setBaudRate(BaudRateType);
    virtual void setDataBits(DataBitsType);
    virtual void setParity(ParityType);
    virtual void setStopBits(StopBitsType);
    virtual void setFlowControl(FlowType);
    virtual void setTimeout(ulong=0, ulong=0);

    virtual bool open(OpenMode mode=0);
    virtual void close();
    virtual void flush();

    virtual qint64 size() const;
    virtual qint64 bytesAvailable();

    virtual bool getChar(char * c);
    virtual bool putChar(char c);
    virtual void ungetChar(char c);

    virtual void translateError(ulong error);

    virtual void setDtr(bool set=true);
    virtual void setRts(bool set=true);
    virtual ulong lineStatus();

    virtual qint64 readData(char * data, qint64 maxSize);
    virtual qint64 writeData(const char * data, qint64 maxSize);

protected:
    QFile* Posix_File;
    struct termios Posix_CommConfig;
    struct timeval Posix_Timeout;
    struct timeval Posix_Copy_Timeout;
};

#endif
