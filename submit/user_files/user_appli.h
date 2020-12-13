#ifndef USER_APPLI_H
#define USER_APPLI_H

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2c-dev.h"
#include <pthread.h>
#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>
#include "widget.h"
#include <QDesktopWidget>
using namespace std;

#define SLV_ADDR 0x52
#define FRAME_SIZE 6
#define OK 1
#define FAIL -1
#define I2C_FILE "/dev/i2c-2"
#define START_BYTE 0x00

#define XMAX 950
#define YMAX XMAX
#define TRAILER 25
#define IDLE_DATA_X 126
#define IDLE_DATA_Y 129
#define SLEEP(x) usleep(x*1000); //sleeps for x milliseconds

#define ORIGIN_X (XMAX + XMIN)/2
#define ORIGIN_Y (YMAX + YMIN)/2 

#define ERROR(val) { \
                     if(val < 0) \
                   { \
                    cout<<"ERROR value" << val << "line "<<  __LINE__ << endl; \
		    return -1; \
		     } \
                    }
#define PRINT_LINE cout<<"line "<< __LINE__<<endl;
#define DECODE(x) ((x & 0xFF) ^ 0x17) + 0x17;

typedef struct
{
  int x_axis;
  int y_axis;
  int ax;
  int ay;
  int az;
  unsigned clear;
  unsigned color;
}data_t;

typedef struct 
{
 int x_pos;
 int y_pos;
}position;


//initialization functions
int init_nunchuck(int);
int set_slave_address(int , int );
int init_i2c(int);

//supporting routines
int nunchuck_get_data(int, unsigned char *);
int process_raw_data(unsigned char* ,data_t *);
int calculate_position(data_t *);

//routines for graphics
float get_coordinate(int , int, int);

//thread functions
void* handle_data(void *);

#endif




