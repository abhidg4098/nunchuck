//copyright: ASU

#include "moving_pt.h" 

int xPrev, xCurr, yPrev, yCurr;
struct position trajectory[TRAILER];
int head, tail;

void shift_loc(struct position a[], int x, int y)
{ 
 a[head].x_pos = x;
 a[head].y_pos = y;
 head = (head + 1) < TRAILER? head + 1 : 0;
 printf("storing %d, %d \n", x, y);
 return;
}

void myKeyboardFunc(unsigned char key, int a, int b )
{
 int x, y;
 printf("%c ", key);
 x = xPrev; //= xCurr;
 y = yPrev; //= yCurr;
 //x = trajectory[head -1].x_pos;
 //y = trajectory[head -1].y_pos;
  switch (key)
  {
    case '6':
      x += X_STEP;
      break;
    case '2':
      y -= Y_STEP;
     break;
    case '4':
      x -= X_STEP;
      break;
    case '8':
      y += Y_STEP;
     break;
    default:
      break;   
  }
      if(x > (XMAX - X_STEP))
        x = XMAX - X_STEP;
      else if (x < XMIN)
        x = XMIN;
      if(y > (YMAX - Y_STEP))
        y = YMAX - Y_STEP;
      else if (y < YMIN)
        y = YMIN;
  xPrev = x;
  yPrev = y;
 shift_loc(trajectory, x,y);
glutPostRedisplay();
}


int main(int argc, char **argv)
{
 xCurr = 0;
 yCurr = 0;
 xPrev = 0;
 yPrev = 0;
glutInit(&argc,argv); 
 
// The image is not animated so single buffering is OK.  
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH ); 
 
// Window position (from top corner), and size (width and hieght) 
glutInitWindowPosition( 10, 20 ); 
glutInitWindowSize( 1000, 1000 ); 
glutCreateWindow( "SimpleDraw - Press space bar to toggle images" ); 

// Initialize OpenGL as we like it.. 
initRendering(); 
  printf(" line = %d \n ", __LINE__); 

// Set up callback functions for key presses 
glutKeyboardFunc( myKeyboardFunc );       // Handles "normal" ascii symbols 
// glutSpecialFunc( mySpecialKeyFunc );	  // Handles "special" keyboard keys 
// Set up the callback function for resizing windows 
printf(" line = %d \n", __LINE__); 
glutReshapeFunc( resizeWindow ); 

 printf(" line = %d \n", __LINE__);  

// Call this for background processing 
// glutIdleFunc( myIdleFunction ); 

// call this whenever window needs redrawing 
printf(" line = %d \n", __LINE__);  

glutDisplayFunc( drawScene ); 

fprintf(stdout, "Press space bar to toggle images; escape button to quit.\n"); 
 
// Start the main loop.  glutMainLoop never returns. 
glutMainLoop(  ); 

return(0);	// This line is never reached. 

}


void drawScene(void)
{
 int i, j;
// Clear the rendering window 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 
// Set drawing color to white 
 
 glColor3f( 1.0, 1.0, 1.0 );	
 glBegin( GL_LINE_STRIP );
 printf(" xprev = %d, xCurr = %d \n", xPrev, xCurr); 
 for( i= head, j =0; j < TRAILER; i++, j++)
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

void resizeWindow(int w, int h) 
{ 
	double scale, center; 
	double windowXmin, windowXmax, windowYmin, windowYmax; 
 
	// Define the portion of the window used for OpenGL rendering. 
	glViewport( 10, 20, w, h );	// View port uses whole window 
 
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

void initRendering(void)
{
  	glEnable ( GL_DEPTH_TEST ); 
}
