#include <linux/types.h>
#include <sys/ioctl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <fcntl.h>

#include "i2c-dev.h"
#include "i2cbusses.h"
#include "util.h"
#define ERROR(val) if (val < 0)\
                   {\
		    printf("ERROR value = %d line = %d,",val, __LINE__);\
		    return -1;\
		     }\
		     
#define RX_BYTES 6
#define SLV_ADDR 0x52
int set_slave_addr(int file, int address, int force);

int main(int argc, char *argv[])
{
char filename[32] = "/dev/i2c-2";
char dest[40] = "/home/cse438/abhi/trial1.dat"; // "/home/abhishekdg/nunchuck/trial.dat";
int file,i,j,data[6],exact_data[6];
FILE *dat_file;
char *buf= (char*)malloc(6);
char *byte = (char*)malloc(32);
memset((void*)byte, 0x00, 32);

__s32 l,k;



file = open(filename,O_RDWR);
dat_file = fopen(dest, "w");


if(file == -1)
	{
	printf("Error opening the file\n");
	return -1;
	}
else
	printf("Opening the file succeeded\n");

 scanf("%d", &i);
//First write the Slave address to the device
   printf("inside %d \n", __LINE__);

i = set_slave_addr(file, SLV_ADDR, 0); 	
ERROR(i)
   printf("inside %d \n", __LINE__);
//l = i2c_smbus_write_byte_data(file, I2C_SLAVE, 0x40);
l = i2c_smbus_write_byte(file, 0x40);
ERROR(l)
   printf("inside %d \n", __LINE__);
l = i2c_smbus_write_byte(file, 0x00);
ERROR(l)
   printf("inside %d \n", __LINE__);
k = 0;
 printf("i = %d", i);
i = 1000;
 //scanf("%d", &k);
while(i > 1)
{
   //scanf("%d", &i);
    //printf("inside %d \n", __LINE__);

      l = i2c_smbus_write_byte(file,0x00);
      ERROR(l)
     //printf("reading now line = %d \n", __LINE__);

	k  = i2c_smbus_read_byte(file);
	//ERROR(k)

        printf("read = %d at line %d\n", k, __LINE__);
if((k == -2) || (k == -1) || (k < 0))
	{
	//printf("I2C transaction failed\n");
	sleep(0.01);
	//return -1;
	}
  byte[0] = (char)k;
/*else 
{
	for (i=0;i<RX_BYTES;i++)
	{
	data[i] = buf[i];
	}
}*/
   printf(" %c line = %d \n", byte[0], __LINE__);
   fprintf(dat_file,"%d \n",k );
   //fputs(byte, dat_file);

/*	for(j=0;j<RX_BYTES;j++)
	{
	exact_data[j] = (data[j] XOR 0x17) + 0x17;
	}	
*/
 i--;
} // while

fclose(dat_file);
close(filename);
} // main

int set_slave_addr(int file, int address, int force)
{
	/* With force, let the user read from/write to the registers
	   even when a driver is also running */
       int errno = 1;
      errno = ioctl(file, I2C_SLAVE, address);
       printf("the return value = %d", errno);

	return 0;
}
