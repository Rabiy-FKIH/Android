#include "stdio.h"
#include "serial_protocol.h"
#include "string.h"


void send_serial_cmd(const int fd, uint8_t id)
{
	uint8_t buffer[2] = {id,0};
	serialPutBuff(fd,buffer,sizeof(buffer));
	uint8_t checks = checksum(buffer,sizeof(buffer));
	serialPutBuff(fd,&checks,sizeof(checks));
}

void send_serial_load(const int fd , uint8_t id , void* load , uint16_t size)
{
	if (size > MAX_UART_DECODE_BUFF)
		return ;

	uint8_t buffer[3] = {id,0,0};
	memcpy(buffer+1,&size,sizeof(size));
	serialPutBuff(fd,buffer,sizeof(buffer));
	serialPutBuff(fd,load,size);
	uint8_t checks = checksum(buffer,sizeof(buffer)) + checksum(load,size) ;
	serialPutBuff(fd,&checks,sizeof(checks));

}




uint8_t serial_decode(const int fd , uint8_t* buffer)
{
	static uint8_t state = 0 ;
	packet_decode* packet = (packet_decode*) buffer;
	static uint8_t timeout = 0;

	switch (state) {
		case 0:
			if (serialDataAvail(fd) >= sizeof(serial_header))
			{
				timeout = 0;
				serialGetBuff(fd,buffer,sizeof(serial_header));
				//printf("id=%d , len=%d \n",packet->header.key,packet->header.len);
				//printf("speed=%d , rpm=%d , temp=%d\n",packet->data[0],packet->data[1],packet->data[2]);
				if (packet->header.len > MAX_UART_DECODE_BUFF)
				{
					serialFlush_I(fd);
					return 0;
				}
				state = 1  ;
				return serial_decode(fd,buffer);
			}
			else {
				return 0 ;
			}
			break;
		case 1:
			if(timeout >SERIAL_PROTOCOL_TIMEOUT)
			{
				state = 0;
				timeout = 0;
				serialFlush_I(fd);
				return 0;
			}
			//printf("%d",packet->header.len);
			if (serialDataAvail(fd) >= packet->header.len)
			{
				serialGetBuff(fd,sizeof(serial_header) +buffer,packet->header.len);
				//printf("id=%d , len=%d \n",packet->header.key,packet->header.len);
				//printf("speed=%d , rpm=%d , temp=%d\n",packet->data[0],packet->data[1],packet->data[2]);
				state = 2;
				return serial_decode(fd,buffer);
				/*state = 0;
				timeout = 0;
				return 1;*/	
			}else							
				timeout++;
			break;
		case 2:
			if(timeout >SERIAL_PROTOCOL_TIMEOUT)
			{
				state = 0;
				timeout = 0;
				serialFlush_I(fd);
				return 0;
			}
			if (serialDataAvail(fd) > 0)
			{
				uint8_t checks;
				serialGetBuff (fd,&checks,1);
				state = 0 ;
				timeout = 0;
				if (checks == checksum(packet,sizeof(serial_header)+ packet->header.len))
					return 1 ;
				else
				{
					serialFlush_I(fd);
					return 0;
				}
			}else
			{
				timeout++;
				return 0 ;
			}
			break;
		default:
			break;
	}
	return 0;
}


uint8_t checksum(void* buffer , uint16_t size){
	uint16_t i = 0;
	uint8_t sum = 0;
	for (i ; i<size ; i++)
	{
		sum += ((uint8_t*)buffer)[i];
	}
	return sum;
}
