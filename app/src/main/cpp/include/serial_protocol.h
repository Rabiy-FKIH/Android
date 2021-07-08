
#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#pragma once
#include "stdint.h"
#include "serial.h"

#define MAX_UART_DECODE_BUFF		0xFF
#define SERIAL_PROTOCOL_TIMEOUT		100


#ifdef __cplusplus
extern "C"{
#endif

typedef struct __attribute__ ((__packed__))
{
	uint8_t		key ;
	uint16_t 	len;
}serial_header;

/*typedef struct {
float speed;
float rpm;
float temperature;
}data_struct;
*/
typedef struct __attribute__ ((__packed__))
{
	serial_header	header;
	uint8_t 		data[MAX_UART_DECODE_BUFF];
}packet_decode;



void send_serial_load(const int fd , uint8_t id , void* load , uint16_t size);
void send_serial_cmd(const int fd, uint8_t id);
uint8_t serial_decode(const int fd , uint8_t* buffer);
uint8_t checksum(void* buffer , uint16_t size);


#ifdef __cplusplus
}
#endif

#endif //SERIAL_PROTOCOL_H
