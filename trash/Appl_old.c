#include "i2c-dev.h"
#include <linux/types.h>
#include <sys/ioctl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include <fcntl.h>

#define RX_BYTES 6
#define SLV_ADDR 0x52

int main(int argc, char *argv[])
{
char filename[32] = "/dev/i2c-dev-async";
char *dest = "/home/abhishekdg/nunchuck/trial.dat";
int file,i,j,data[6],exact_data[6];
FILE *dat_file;
char *buf= (char*)malloc(6);
__s32 l,k;

if(argc != 0)
 dest = argv[0];


file = open(filename,O_RDWR);
dat_file = fopen(dest, "a+");


if(file == -1)
	{
	printf("Error opening the file\n");
	return -1;
	}
else
	printf("Opening the file succeeded\n");

 scanf("%d", &i);
 	
l = i2c_smbus_write_byte_data(file, I2C_SLAVE, SLV_ADDR);
 
 scanf("%d", &i);
if (l==0)

{
while(1)
{
   scanf("%d", &i);
	k = i2c_smbus_read_block_data(file, I2C_SMBUS_BLOCK_DATA, buf);

if((k == -2) || (k == -1))
	{
	printf("I2C transaction failed\n");
	sleep(5);
	//return -1;
	}

else

{
	for (i=0;i<RX_BYTES;i++)
	{
	data[i] = buf[i];
	}
}
   fputs(data, dat_file);

/*	for(j=0;j<RX_BYTES;j++)
	{
	exact_data[j] = (data[j] XOR 0x17) + 0x17;
	}	
*/
} // while

} // if (l==0)

} // main


