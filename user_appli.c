#include "user_appli.h"
#include "queue.h"

/*
void drawScene(void);
void resizeWindow(int , int );
void shift_loc(position *, int, int);
*/

/*Routines to handle the nunchuck*/

/*Message mailbox for inter*/
static queue_t *mailbox; 
pthread_mutex_t mutex_q1 = PTHREAD_MUTEX_INITIALIZER;

static int xPrev, xCurr, yPrev, yCurr;
data_t curr_status;
position trajectory[TRAILER];
static int t_head, t_tail, data_ready, only_once = 0;
 Display *display;
 FILE *file_h = NULL;

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

  ret = init_nunchuck(fh);
    ERROR(ret);

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
    ERROR(ret);

  ret = i2c_smbus_write_byte (fh, 0x00);
    ERROR(ret);

  printf("initializing the Nunchuck is Success \n");
  return OK;
}

/**************************************************************
  function: nunchuck_get_data()
  inputs  : file handle, buffer to store data
  returns : OK = 1 when success else  -1
  description : Recieves the data byte by byte, to get 6 byte frame
*****************************************************************/

int nunchuck_get_data(int file_handle, int *buffer)
{
  int i,ret;
  char str[8];
  /*Must gather 6 bytes at any cost this time*/
  for(i=0; i < FRAME_SIZE; i++)
  {
   #if 0
   
      buffer[i] = -1;
      /*Sending the start byte to the device*/
      ret = i2c_smbus_write_byte (file_handle, START_BYTE);
     
     if (ret == 0)
     {
       /*Try until a valid byte is read from device*/
       while(buffer[i] < 0)
       {
         buffer[i] = i2c_smbus_read_byte(file_handle);
         printf("data = %d \n", buffer[i]);
         /*let the device recuperate after the transaction*/
	 SLEEP(100);
        }
     }
    else
    {
     /*Do not give up, try again */
     printf("Could not write the start byte, try again \n");
     i--;
    } 
  #else

   if (fgets(str, 8, file_h))
   {
     buffer[i] = atoi(str);
   }
   else
     return FAIL;

   printf("%d \n", buffer[i]);
   SLEEP(100);
  #endif
  }
  /*reporting the success*/
  return OK;
 }

/**************************************************************
  function: process_raw_data()
  inputs  : file handle, buffer to store data
  returns : OK = 1 when success else  -1
  description : processes the recieved frame
*****************************************************************/

int process_raw_data(int *buff, data_t *element)
{
  int var,i;

  if((NULL == buff) || (NULL == element))
    return FAIL;

  for(i = 0; i < FRAME_SIZE; i++) 
  {
    var = buff[i];
    var = DECODE(var);
    //here IDLE data is obtained when there is nothing for nunchuck to send 
    //removing the offset.
    var -= IDLE_DATA;  
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
        element->color = (var & 0x02) ? TRUE : FALSE;
        element->clear = (var & 0x01) ? TRUE : FALSE;
      break;
      default:
        break;
    }
  }
  /*The function doesnot require a return value*/
  return OK;  
}

/**************************************************************
  function: shift_loc()
  inputs  : circular queue buffer and coordinates
  returns : NA
  description : inserts an entry into circular buffer
*****************************************************************/

void shift_loc(position *a, int x, int y)
{ 
 /*Circular buffer to keep track of positions*/
 a[t_head].x_pos = x;
 a[t_head].y_pos = y;
 t_head = (t_head + 1) < TRAILER? (t_head + 1) : 0;
 return;
}

/**************************************************************
  function: drawScene()
  inputs  : data_t
  returns : NA
  description : plots the coordinates on the screen
*****************************************************************/

void drawScene( void)
{
 int i, j;
 float k[3] = {1.0, 1.0, 1.0};
 static char index = 1;
 data_t action = curr_status;
 // Clear the rendering window 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

 //Clear the buffer and image when C button is presssed
 if(action.clear != 0)
  {
    for(i = 0; i < TRAILER; i++)
     {
       trajectory[i].x_pos = (int)ORIGIN_X;
       trajectory[i].y_pos = (int)ORIGIN_Y;
     } 
  }
 

 //toggling the colour as with each press of Z button
 //if(action.color != 0)
 {
   index = (index + 1) % 8;
   index = (index == 0) ? 1 : index;
   k[2] = (index >> 2) & 0x01;
   k[1] = (index >> 1) & 0x01;
   k[0] = index & 0x01;
   printf("/************************ colour bit set ************************/ \n");
   glColor3f( k[0], k[1], k[2]);	
 }
 glBegin( GL_LINE_STRIP );

 printf(" xprev = %d, xCurr = %d \n", xPrev, xCurr); 
 for( i= t_head, j =0; j < TRAILER; i++, j++)
 {
   if(i == TRAILER)
   {
     i = 0;
   }
   printf("(%d, %d)\t", trajectory[i].x_pos ,trajectory[i].y_pos); 
   glVertex2f(trajectory[i].x_pos ,trajectory[i].y_pos);
    
 }
 printf("\n");
 glEnd();

 glFlush();
}

/**************************************************************
  function: resizeWindow()
  inputs  : sets Windw dimensions
  returns : OK = 1 when success else  -1
  description : 
*****************************************************************/

void resizeWindow(int w, int h) 
{ 
	double scale, center; 
	double windowXmin, windowXmax, windowYmin, windowYmax; 
 
	// Define the portion of the window used for OpenGL rendering. 
	glViewport( ORIGIN_X, ORIGIN_Y, w, h );	// View port uses whole window 
 
	// Set up the projection view matrix: orthographic projection 
	// Determine the min and max values for x and y that should appear in the window. 
	// The complication is that the aspect ratio of the window may not match the 
	//		aspect ratio of the scene we want to view. 
	//w = (w==0) ? 1 : w; 
	//h = (h==0) ? 1 : h; 
        w = 1;
        h = 1;
       printf(" line = %d \n", __LINE__);  

	if ( (XMAX-XMIN)/w < (YMAX-YMIN)/h ) { 
		scale = ((YMAX-YMIN)/h)/((XMAX-XMIN)/w); 
		center = (XMAX+XMIN)/2; 
		windowXmin = center - (center-XMIN)*scale; 
		windowXmax = center + (XMAX-center)*scale; 
		windowYmin = YMIN; 
		windowYmax = YMAX; 
printf(" line = %d \n", __LINE__); 
	} 
	else { 
		scale = ((XMAX-XMIN)/w)/((YMAX-YMIN)/h); 
		center = (YMAX+YMIN)/2; 
		windowYmin = center - (center-YMIN)*scale; 
		windowYmax = center + (YMAX-center)*scale; 
		windowXmin = XMIN; 
		windowXmax = XMAX; 
 printf(" __LINE__  \n"); 

	} 
	 
	// Now that we know the max & min values for x & y that should be visible in the window, 
	//		we set up the orthographic projection. 
	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity(); 
	glOrtho( windowXmin, windowXmax, windowYmin, windowYmax, -1, 1 ); 
}


float get_coordinate(int vel, int acc, int prev)
{
  float s = 0;

  /* calculate along axis */
  /* using s = u*t + 1/2 a*t*t */
  //s = vel/DELAY + ((0.5)*acc)/(DELAY*DELAY);
  s= vel + 0.5*acc/DELAY*DELAY;

  printf("values %f \n", s);
  //s = s*2; //from 250 to 500
  s = prev + s;

  return s;
}


int calculate_position(data_t *p_in)
{
  float value;
  /* get xPrev and yPrev to calculate the change */
  printf("values %d, %d, %d \n",  p_in->x_axis, p_in->ax, xPrev);
  value = get_coordinate(p_in->x_axis, p_in->ax, xPrev);
  xPrev = (int)value; 

  value = get_coordinate(p_in->y_axis, p_in->ay, yPrev);
  yPrev = (int)value;
  
}

void generate_event(void)
{
 unsigned int keycode;
 display = XOpenDisplay(NULL);
 keycode = XKeysymToKeycode(display, XK_A);
 XTestFakeKeyEvent(display, keycode, True, 0);
 XTestFakeKeyEvent(display, keycode, False, 0);
 XFlush(display);
  if(display != NULL)
  {
   XCloseDisplay(display); 
   display = NULL;
  }
 return;
}

void display_again(unsigned char key)
{
  glutPostRedisplay();
  return;
}

void *handle_data(void *p_entry_data)
{
  printf("Polling for the data \n");
  
  int file_handle = *((int*)p_entry_data);
  data_t *container =  malloc(sizeof (data_t));            //temporary variable
  int *buff = (int*)malloc(2*FRAME_SIZE);
   
  int status,idx;
  float delay = 1/DELAY;

  memset((void*)buff, 0x00, FRAME_SIZE);

//PRINT_LINE
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
    printf("x_accl= %d, y_accl = %d \n",container->ax, container->ay );
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
}

void* handle_display(void *p_in)
{
  data_t new_data;
  int i,ret = 0;
 
  for(i = 0; i < TRAILER; i++)
  {
      trajectory[i].x_pos = (int)ORIGIN_X;
      trajectory[i].y_pos = (int)ORIGIN_Y;
  } 
      xPrev = ORIGIN_X;
    yPrev = ORIGIN_Y;
  while(1)
  {
    /*poll the queue*/
   // while(1 != dequeue(mailbox, &new_data));
    do{
    ret = dequeue(mailbox, &new_data);
    printf("dequeing mailbox->count %d\n", mailbox->count);
    SLEEP(600);
     }while(ret < 0);
    printf(" after dequeue : %d %d %d %d \t ", new_data.x_axis, new_data.y_axis, new_data.ax, new_data.ay);
    /* calculate positions and store it in xPrev/yPrev */
    calculate_position(&new_data);
    shift_loc(trajectory, xPrev, yPrev);
    data_ready = 1;
    
    /* plot positions */   

    glutPostRedisplay();
  }
}
void* event_handler(void *p)
{
  while(1)
  {
    if(1)
    {
     generate_event();
     data_ready = 0;
    }
    SLEEP(600); //sleep(1);
  }

}

int main(int argc, char *argv[])
{
 pthread_t extractor_thread, graphics_thread, event_thread;
 int file_handle,status,ret,i;

 file_h = fopen("/home/abhishekdg/nunchuck/circular.dat", "r");
 mailbox = (queue_t*)malloc(sizeof(queue_t));
   if(file_h == NULL)
        return -1;

 //open the device file after the module is loaded
 file_handle = open(DEFAULT_FILE, O_RDWR);

#if 0
 //check if the file was opened with success
 if(file_handle == -1)
  {
    printf("Error opening the device file\n");
    free(mailbox);
    return -1;
  }
 else
   printf("Opening the device file succeeded\n");

  /* initialize the device with address and handshake */
  status = init_i2c(file_handle);
   ERROR(status)
#endif
  /* initialize the message queue */
  init(mailbox);

  /*Initialize graphics : glut library */
  glutInit(&argc,argv); 
 
  // The image is not animated so single buffering is OK.  
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH ); 
 
  // Window position (from top corner), and size (width and hieght) 
  glutInitWindowPosition( ORIGIN_X, ORIGIN_Y ); 
  glutInitWindowSize( XMAX, YMAX ); 
  glutCreateWindow( "NUNCHUCK trajectory" ); 

  // Initialize OpenGL .. 
  glEnable ( GL_DEPTH_TEST );  
  glutKeyboardFunc( display_again );       // Handles keyboard event
  // callback function for resizing windows 
  glutReshapeFunc( resizeWindow );
  glutDisplayFunc( drawScene ); 
  /*start the application threads*/

  printf("Creating the threads \n");
  if( only_once == 0)
  {
    pthread_create(&extractor_thread, NULL,  &handle_data,(void*)&file_handle);
    pthread_create(&graphics_thread, NULL, &handle_display, NULL);
    pthread_create(&event_thread, NULL, &event_handler, NULL);
    only_once = 1;
  }
  glutMainLoop();
  pthread_join(extractor_thread, NULL);
  pthread_join(graphics_thread, NULL);
  pthread_join(event_thread, NULL);


 free(mailbox);
 return 0;

}



//###########################




















