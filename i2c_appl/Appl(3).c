#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2c-dev.h"
#include "i2cbusses.h"

#define SLV_ADDR 0x52
#define false 0
#define true 1

int init_nunchuck();

int main(int argc, char **argv[])
{
int a,status,ret,i;
char *buffer= (char*)malloc(256);
FILE *dat_file;
a = open("/dev/i2c-2", O_RDWR);
dat_file = fopen("/home/cse438/Pavan/trial.dat", "a+");
status = 1;

if(a == -1)
	{
	printf("Error opening the file\n");
	return -1;
	}
else
	printf("Opening the file succeeded\n");

int init_i2c();

while (status)
{
status = false;
ret = nunchuck_get_data(buffer);
	if (ret > 0)
	{
	status = true;
	fputs (buffer,dat_file);
       // printf("%d",buffer);
	}
}
}



init_i2c()
{
int ret,a;
ret = set_slave_addr(a, SLV_ADDR, 0);
if (ret < 0)
return -1;
ret = init_nunchuck();
return ret;
}



init_nunchuck()
{
int ret,a;
ret = i2c_smbus_write_byte (a, 0x40);
if (ret < 0) 
return -1;

ret = i2c_smbus_write_byte (a, 0x00);
if (ret < 0) 
return -1;

return +1;
}



int nunchuck_get_data(char *buffer)
{
 //buffer[] = {0,0,0,0,0,0};
int i,a,ret;
FILE *dat_file;

for(i=0;i<6;i++)
{
ret = i2c_smbus_write_byte (a, 0x00);
	if (ret == 0)
	buffer[i] = i2c_smbus_read_byte(a);
	if (buffer[i] < 0)
	return -1;
}

return +1;
}

int set_slave_addr(int file, int address, int force)
{
int errno;
	/* With force, let the user read from/write to the registers
	   even when a driver is also running */
	if (ioctl(file, force ? I2C_SLAVE_FORCE : I2C_SLAVE, address) < 0) {
		fprintf(stderr,
			"Error: Could not set address to 0x%02x: %s\n",
			address, strerror(errno));
		return -errno;
	}

	return 0;
}





