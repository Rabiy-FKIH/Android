//
// Created by rabiy on 5/6/21.
//

/*
 * wiringSerial.c:
 *	Handle a serial port
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "serial.h"

/*
 * serialOpen:
 *	Open and initialise the serial port, setting all the right
 *	port parameters - or as many as are required - hopefully!
 *********************************************************************************
 */

int serialOpen (const char *device, const int baud)
{
    struct termios options ;
    speed_t myBaud ;
    int     status, fd ;

    switch (baud)
    {
        case      50: myBaud =      B50 ; break ;
        case      75: myBaud =      B75 ; break ;
        case     110: myBaud =     B110 ; break ;
        case     134: myBaud =     B134 ; break ;
        case     150: myBaud =     B150 ; break ;
        case     200: myBaud =     B200 ; break ;
        case     300: myBaud =     B300 ; break ;
        case     600: myBaud =     B600 ; break ;
        case    1200: myBaud =    B1200 ; break ;
        case    1800: myBaud =    B1800 ; break ;
        case    2400: myBaud =    B2400 ; break ;
        case    4800: myBaud =    B4800 ; break ;
        case    9600: myBaud =    B9600 ; break ;
        case   19200: myBaud =   B19200 ; break ;
        case   38400: myBaud =   B38400 ; break ;
        case   57600: myBaud =   B57600 ; break ;
        case  115200: myBaud =  B115200 ; break ;
        case  230400: myBaud =  B230400 ; break ;

        case  500000: myBaud =  B500000 ; break ;  // MT
        case 1000000: myBaud = B1000000 ; break ;  // MT
        case 2000000: myBaud = B2000000 ; break ;  // MT

        default:
            return -2 ;
    }

    if ((fd = open (device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK)) == -1)
        return -1 ;

    fcntl (fd, F_SETFL, O_RDWR) ;

// Get and modify current options:

    tcgetattr (fd, &options) ;

    cfmakeraw   (&options) ;
    cfsetispeed (&options, myBaud) ;
    cfsetospeed (&options, myBaud) ;
    //
    //fgetospeed() returns the output baud rate stored in the termios structure pointed to by termios_p.

    options.c_cflag |= (CLOCAL | CREAD) ;
    options.c_cflag &= ~PARENB ;
    options.c_cflag &= ~CSTOPB ;
    options.c_cflag &= ~CSIZE ;
    options.c_cflag |= CS8 ;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG) ;
    options.c_oflag &= ~OPOST ;

    options.c_cc [VMIN]  =  0 ;
    options.c_cc [VTIME] = 10 ; // One second (10 deciseconds) // MT was 100

    tcsetattr (fd, TCSANOW | TCSAFLUSH, &options) ;

    ioctl (fd, TIOCMGET, &status);

    status |= TIOCM_DTR ;
    status |= TIOCM_RTS ;

    ioctl (fd, TIOCMSET, &status);

    usleep (10000) ;	// 10mS

    return fd ;
}


/*
 * serialFlush_I:
 *	Flush the serial buffer ( rx )
 *********************************************************************************
 */

void serialFlush_I (const int fd)
{
    tcflush (fd, TCIFLUSH) ;	//TCIFLUSH : it flushes data received but not read. 
}

/*
 * serialFlush_O:
 *	Flush the serial buffer ( tx )
 *********************************************************************************
 */

void serialFlush_O (const int fd)
{
    tcflush (fd, TCOFLUSH) ;	//TCOFLUSH :it flushes data written but not transmitted. 
}
/*
 * serialFlush_IO:
 *	Flush the serial buffers (both tx & rx)
 *********************************************************************************
 */

void serialFlush_IO (const int fd)
{
    tcflush (fd, TCIOFLUSH) ;	//TCIOFLUSH : it flushes both data received but not read and data written but not transmitted. 
}

/*
 * serialClose:
 *	Release the serial port
 *********************************************************************************
 */

void serialClose (const int fd)
{
    close (fd) ;
}



/*
 * serialPutchar:
 *	Send a single character to the serial port
 *********************************************************************************
 */

void serialPutchar (const int fd, const unsigned char c)
{
    write (fd, &c, 1) ;
}

/*
 * serialPuts:
 *	Send a string to the serial port
 *********************************************************************************
 */

void serialPuts (const int fd, const char *s)
{
    write (fd, s, strlen(s)) ;
}


/*
 * serialPuts:
 *	Send a buffer to the serial port
 *********************************************************************************
 */

void serialPutBuff (const int fd, void*  buffer,int size)
{
    write (fd, buffer, size) ;
}

/*
 * serialPrintf:
 *	Printf over Serial
 *********************************************************************************
 */

void serialPrintf (const int fd, const char *message, ...)
{
    va_list argp ;
    char buffer [1024] ;

    va_start (argp, message) ;
    vsnprintf (buffer, 1023, message, argp) ;
    va_end (argp) ;

    serialPuts (fd, buffer) ;
}


/*
 * serialDataAvail:
 *	Return the number of bytes of data available to be read in the serial port
 *********************************************************************************
 */

int serialDataAvail (const int fd)
{
    int result ;

    if (ioctl (fd, FIONREAD, &result) == -1)
        return -1 ;

    return result ;
}


/*
 * serialGetchar:
 *	Get a single character from the serial device.
 *	Note: Zero is a valid character and this function will time-out after
 *	10 seconds.
 *********************************************************************************
 */

int serialGetchar (const int fd)
{
    uint8_t x ;

    if (read (fd, &x, 1) != 1)
        return -1 ;

    return ((int)x) & 0xFF ;
}

/*
 * serialGetBuff:
 *	attempts to read up to sizeof(buff) bytes from file descriptor fd
       into the buffer starting at buf.
 *********************************************************************************
 */

int serialGetBuff (const int fd,void* buffer,int size)
{
    return read (fd, buffer,size);
}

