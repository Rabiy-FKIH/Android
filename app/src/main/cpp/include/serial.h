//
// Created by rabiy on 5/6/21.
//

#ifndef SERIAL_H
#define SERIAL_H

/*
 * wiringSerial.h:
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

#ifdef __cplusplus
extern "C" {
#endif

extern int	serialOpen      (const char *device, const int baud);
extern void	serialClose     (const int fd);
extern void	serialFlush_I   (const int fd);
extern void	serialFlush_O   (const int fd);
extern void	serialFlush_IO	(const int fd);
extern void	serialPutchar	(const int fd, const unsigned char c);
extern void	serialPuts	    (const int fd, const char *s);
extern void	serialPutBuff 	(const int fd, void*  buffer,int size);
extern void	serialPrintf	(const int fd, const char *message, ...);
extern int	serialDataAvail (const int fd);
extern int	serialGetchar	(const int fd);
extern int	serialGetBuff 	(const int fd,void* buffer,int size);

#ifdef __cplusplus
}
#endif

#endif //SERIAL_H

