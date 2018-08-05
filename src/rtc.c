#include "rtc.h"
#include "i2c.h"
#include "hardware.h"
#include <string.h>


#define OFFSET 0x00


void RTC_Read(TIME* ptime)
{
	char addr = OFFSET;
	unsigned char buf[sizeof(TIME)+1];
	unsigned char* p = (unsigned char*)ptime;
	I2C_MSG cmd[2];
	
	cmd[0].slaveaddr =    0xD0;
	cmd[0].flags     =    (I2CFlagStart | I2CFlagWrite);
	cmd[0].pbuffer   =    &addr;
	cmd[0].cbbuffer  =    1;
	
	cmd[1].slaveaddr =    0xD0;
	cmd[1].flags     =    (I2CFlagStart | I2CFlagRead);
	cmd[1].pbuffer   =    (unsigned char*)buf;
	cmd[1].cbbuffer  =    sizeof(buf);
	
	I2C_Command(cmd,2);

	memcpy(p,buf,3);
	memcpy(p+3,buf+4,3);	
		   
}


void RTC_Write(const TIME* ptime)
{
	char temp[8];
	I2C_MSG cmd;
    unsigned char* p1 = ptime;
	unsigned char* p2 = temp; 

	cmd.slaveaddr =    0xD0;
	cmd.flags     =    (I2CFlagStart | I2CFlagWrite);
	cmd.pbuffer   =    temp;
	cmd.cbbuffer  =    sizeof(temp);
	
	*p2++ = OFFSET;
	memcpy(p2,p1,3);
	p2+=3;
	*p2++ = 0x00;
	p1+=3;
	memcpy(p2,p1,3);
	
	I2C_Command(&cmd,1);
  			
}

