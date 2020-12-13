#include<stdio.h>
#include<vga.h>
#include<stdlib.h>

void draw_line(int, int, int, int);

//Draw the coordinate system for a 640x480 resolution. Could be generalized for other resolutions
void draw_axis(){
  vga_drawline(319,0,319,478);
  vga_drawline(0,239,638,239);
}

int main()
{
 int x1,x2,y1,y2;

vga_init(); //Initialize the Graphics array

 /*printf("Enter coordinates (x1, y1): ");
  scanf("%d %d", &x1, &y1);

 printf("Enter coordinates (x2, y2): ");
  scanf("%d %d", &x2, &y2);*/
 x1=35;
 x2= 240;
 y1=35;
 y2= 240;
 vga_setmode(5); //This mode sets the screen resolution of the graphics array to 640x480 pixels
 draw_axis(); //Call to function that displays the four quadrants (coordinate system)
 draw_line(x1, y1, x2, y2);

 vga_getch(); //Wait for a key from the user to quit the graphics array
 vga_setmode(0); //Set the screen mode back to text mode. 

 // Typically useful, when the user will run this program from the console and will be left back on to 
 // the text mode when the program exits. A good practise to do so.

 printf("\n");

 return 0;

}

void draw_line(int x1, int y1, int x2, int y2)
{	
  vga_drawline(x1+319, abs(y1-239), x2+319, abs(y2-239));	
}

