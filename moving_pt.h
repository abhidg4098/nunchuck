//copyright: ASU

#include <stdlib.h> 
#include <stdio.h> 
#include <GL/glut.h>	// OpenGL Graphics Utility Library 

#define XMIN 0
#define XMAX 400
#define YMIN XMIN
#define YMAX 400
#define X_STEP XMAX/20
#define Y_STEP YMAX/20 
#define TRAILER 40

typedef struct position
{
 int x_pos;
 int y_pos;
}; 

//void myKeyboardFunc( unsigned char key, int a, int b );

void drawScene(void);

void initRendering();
void resizeWindow(int w, int h);

