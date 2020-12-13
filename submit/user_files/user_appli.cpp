#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>
#include "widget.h"
#include <QDesktopWidget>
#include "user_appli.h"
#include "queue.h"

/*Routines to handle the nunchuck*/

/*Message mailbox for inter*/
queue_t *mailbox; 
pthread_mutex_t mutex_q1 = PTHREAD_MUTEX_INITIALIZER;

int xPrev,yPrev;
data_t curr_status;

/**************************************************************
  function: set_slave_address()
  inputs  : file handle, address of slave
  returns : OK = 1 when success else  -1
  description : forces the slave address on i2c bus to given value
*****************************************************************/
int set_slave_address(int file, int address)
{
  int val;
  /* Set the address of the slave  */
  val = ioctl(file, I2C_SLAVE, address); 
      ERROR(val);

  return OK;
}

/**************************************************************
  function: init_i2c()
  inputs  : file handle, address of slave
  returns : OK = 1 when success else  -1
  description : forces the slave address and initializes nunchuck
*****************************************************************/
int init_i2c(int fh)
{
  int ret;

  ret = set_slave_address(fh, SLV_ADDR);
    ERROR (ret)
  SLEEP(100);
  ret = init_nunchuck(fh);
    ERROR(ret)

  printf("Successfully registered the Slave \n");

  return OK;
}

/**************************************************************
  function: init_nunchuck()
  inputs  : file handle, address of slave
  returns : OK = 1 when success else  -1
  description : initializes the nunchuck with 0x40 and 0x00 bytes
*****************************************************************/

int init_nunchuck(int fh)
{
  int ret;

  ret = i2c_smbus_write_byte (fh, 0x40);
    ERROR(ret)

  ret = i2c_smbus_write_byte (fh, 0x00);
    ERROR(ret)

  SLEEP(100);

  printf("initializing the Nunchuck is Success \n");
  return OK;
}

/**************************************************************
  function: nunchuck_get_data()
  inputs  : file handle, buffer to store data
  returns : OK = 1 when success else  -1
  description : Recieves the data byte by byte, to get 6 byte frame
*****************************************************************/

int nunchuck_get_data(int file_handle, unsigned char *buffer)
{
  int i,ret,var;
  ret = i2c_smbus_write_byte (file_handle, START_BYTE);
  ERROR(ret)

   do
    {
     /* remove the previous byte */
     var = i2c_smbus_read_byte(file_handle);
     SLEEP(100);
    }while(var < 0);

  /*Must gather 6 bytes at any cost this time*/
  for(i=0; i < FRAME_SIZE; i++)
  {
      buffer[i] = 0;
      /*Sending the start byte to the device*/
     if (ret == 0)
     {
       /*Try until a valid byte is read from device*/
       do
       {
         var = i2c_smbus_read_byte(file_handle);
         /*let the device recuperate after the transaction*/
         SLEEP(10);
        }while(var < 0);
       buffer[i] = DECODE(var);
     }
    else
    {
     /*Do not give up, try again */
     printf("Could not write the start byte, try again \n");
     i--;
    } 
  }
  return OK;
 }

/**************************************************************
  function: process_raw_data()
  inputs  : file handle, buffer to store data
  returns : OK = 1 when success else  -1
  description : processes the recieved frame and update the fileds
                we store all possible data into our structure.
*****************************************************************/

int process_raw_data(unsigned char *buff, data_t *element)
{
  int var,i;

  if((NULL == buff) || (NULL == element))
    return FAIL;

  for(i = 0; i < FRAME_SIZE; i++) 
  {
    var = buff[i];
   
    switch(i)
    {
      case 0:
        element->x_axis = var;
      break;
      case 1:
        element->y_axis = var;
      break;
      case 2:
        element->ax = var << 2;
      break;
      case 3:
        element->ay = var << 2;
      break;
      case 4:
        element->az = var << 2;
      break;
      case 5:
        element->az += (var >> 6) & 0x03;
        element->ay += (var >> 4) & 0x03;
        element->ax += (var >> 2) & 0x03;
        var = (var & 0x03);
        curr_status.color = 0;
        curr_status.clear = 0;

        //z = 01, c = 00, nothing =10, both = 11
        curr_status.color = (var == 0x01) || (var == 0x03);
        curr_status.clear = (var == 0x00) || (var == 0x03);

      break;
      default:
        break;
    }
  }
  /*The function doesnot require a return value*/
  return OK;  
}

/**************************************************************
  function: get_coordinate()
  inputs  : velocity, previous value, offset
  returns : new value of the position
  description : remove the offset,  scale and clip the
                calculated coordinate
*****************************************************************/
float get_coordinate(int vel, int prev, int offset)
{
  float s = 0;
  /* calculate along axis */
  s = prev + (vel - offset)/4;

  if(s < 0)
      s = 0;
  else if(s > XMAX)
      s = XMAX;

  return s;
}

/**************************************************************
  function: calculate_position()
  inputs  : current attributes from the nunchuck
  returns : OK = 1 when success  always
  description : processes the recieved frame, estimate coordinates
                usinf x/y axis velocities
*****************************************************************/
int calculate_position(data_t *p_in)
{
  /* get xPrev and yPrev to calculate the change */
  xPrev = (int)get_coordinate(p_in->x_axis, xPrev, IDLE_DATA_X);
  yPrev = (int)get_coordinate(-p_in->y_axis, yPrev, -IDLE_DATA_Y);
  return OK;
}

/**************************************************************
  function: handle_data()
  inputs  : entry data
  returns : NULL on kill
  description : gets data from the device pereiodically processes
                and stores it in local structure and enqueues it
                in the mailbox.
*****************************************************************/
void *handle_data(void *p_entry_data)
{
  printf("Thread 1 :Polling device for the data to be put to mailbox\n");
  
  int file_handle = *((int*)p_entry_data);
  data_t *container =  (data_t*)malloc(sizeof (data_t));
  unsigned char *buff = (unsigned char *)malloc(6*sizeof(unsigned char));
   
  int status;

  memset((void*)buff, 0x00, FRAME_SIZE);

  while(1)
  {
   /*get the data from nunchuck*/
    do 
    {
      /* 6 bytes at a time */
      /* try until all 6 bytes are recieved correctly */
      status = nunchuck_get_data(file_handle,buff);

     } while(status < 0);

   /*process the recieved data */
    status = process_raw_data(buff, container);
     if(status < 0) 
        continue;
    SLEEP(100);
    /* lock the queue until queuing finishes */
    while(pthread_mutex_trylock(&mutex_q1));
    /*store it to the queue */  
    status = enqueue(mailbox, *container);
    pthread_mutex_unlock(&mutex_q1);   


    if(status < 0) 
      printf("Error inserting the element to the queue \n");
  }
  free(container);
  free(buff);
  return NULL;
}
/**************************************************************
  function: center()
  inputs  : Qwidget
  returns : void
  description : initilize the display screen for the user
*****************************************************************/
void center(QWidget &widget)
{
  int x, y;
  int screenWidth;
  int screenHeight;

  QDesktopWidget *desktop = QApplication::desktop();

  screenWidth = desktop->width();
  screenHeight = desktop->height();

  x = (screenWidth - XMAX) / 2;
  y = (screenHeight - YMAX) / 2;

  widget.setGeometry(x, y, XMAX, YMAX);
  return;
}

/**************************************************************
  function: main()
  inputs  : argc, argv[][]
  returns : OK = 1 when success else  -1
  description : opens device file, creates threads to retrieve
                data from device and plots the data on screen
*****************************************************************/
int main(int argc, char *argv[])
{
 pthread_t extractor_thread;
 QApplication trajectory(argc, argv);
 update_thread tline;
 int file_handle,status;

 tline.setWindowTitle("Lines");
 tline.show();
 center(tline);
 tline.clearDisplay();

 //open the device file after the module is loaded
 file_handle = open(I2C_FILE, O_RDWR);

 //check if the file was opened with success
 if(file_handle == -1)
  {
    printf("Error opening the device file\n");
    return -1;
  }
 else
   printf("Opening the device file succeeded\n");

  /* initialize the device with address and handshake */
  status = init_i2c(file_handle);
  if(status != OK )
  {
    cout<<"could not initialize the nunchuck, device not replying!" <<endl;
    return -1;
  }
 /*start the application threads*/
  printf("Creating the threads \n");

   /* initialize the message queue */
    mailbox = (queue_t*)malloc(sizeof(queue_t));
    init(mailbox);
    pthread_create(&extractor_thread, NULL,  &handle_data,(void*)&file_handle);
    tline.start();

  tline.update();
  trajectory.exec();
  pthread_join(extractor_thread, NULL);
  
 free(mailbox);
 return 0;

}

//###########################




















