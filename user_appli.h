#ifndef USER_APPLI_H
#define USER_APPLI_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include "i2c-dev.h"
#include "i2cbusses.h"
#include <pthread.h>
#include <GL/glut.h>	// OpenGL Graphics Utility Library 
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#define SLV_ADDR 0x52
#define FRAME_SIZE 6
#define FALSE 0
#define TRUE 1
#define OK 1
#define FAIL -1
#define DEFAULT_FILE "/dev/i2c-2"
#define DELAY 0x10
#define START_BYTE 0x00

#define XMIN 0
#define XMAX 500
#define YMIN XMIN
#define YMAX 500
#define X_STEP XMAX/20
#define Y_STEP YMAX/20 
#define TRAILER 25
#define IDLE_DATA 0x81
#define SLEEP(x) usleep(x*1000); //sleeps for x milliseconds

#define X_FACTOR XMAX/250
#define Y_FACTOR YMAX/250
#define ORIGIN_X (XMAX + XMIN)/2
#define ORIGIN_Y (YMAX + YMIN)/2 

#define ERROR(val) { \
                     if(val < 0) \
                   { \
		    printf("ERROR value = %d line = %d \n",val, __LINE__); \
		    return -1; \
		     } \
                    }
#define PRINT_LINE printf("line = %d", __LINE__);
#define DECODE(x) (x ^ 0x17) + 0x17;

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
int nunchuck_get_data(int, int *);
int process_raw_data(int* ,data_t *);
int calculate_position(data_t *);

//routines for graphics
void drawScene(void);
void resizeWindow(int , int );
void shift_loc(position *, int, int);
float get_coordinate(int , int, int);
void generate_event(void);
void display_again(unsigned char);

//thread functions
void* handle_data(void *);
void* handle_display(void *);
void* event_handler(void *);

#endif




