#include "client.h"
#include <GL/glew.h> 
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <sys/mman.h>
#include <errno.h>
#include <vector>
// #include "./Src/PixelBuffer.h"
// #include "clientAuthWindow.h"
const int window_width{1366 } ;
const int window_height{786};

void render();
void timer(int x);
void keyboard(unsigned char c , int x , int y);
void mouse(int button , int state , int x , int y);
int handler(int argc, char** argv);
void processSpecialKeys(int key, int x, int y);
void drawPixels(unsigned char * data , int x , int y , int w , int h);

// Client C();

Client C;

int main(int argc, char** argv){
    
   // if(openingWindow(argc,argv) == false){ 
    //     printf("unauthorised client\n" );
    //     return 1;
    // } 
    // printf("successfull authorization \n");
    // printf("create connection\n");
    // //create connection
    // printf("connection created successfully\n");
    C.initiateConnection();

    handler(argc , argv) ; 
    return 0;
}

void drawPixels(unsigned char * data , int x , int y , int w , int h){
    glBegin (GL_POINTS);
        for (int yy = 0; yy < h; yy++) {
            for (int xx = 0; xx < w; xx++) {
                glColor3f((float)(data[(yy*w+xx)*3+0]) / 255,(float)(data[(yy*w+xx)*3+1]) / 255,(float)(data[(yy*w+xx)*3+2]) / 255);
                glVertex2i( xx + x , yy + y );

            }
        }
    glEnd ();


}


void render(){
    
    while (!C.frameQs.empty()){
        FrameBufferUpdate F;
        int size = -1;
            F = C.frameQs.front();
            size = C.frameQs.size();
            C.frameQs.pop();
        
        if (size>0 && F.rectangleResponse[0].information[0]!='\0'){

            glMatrixMode( GL_PROJECTION );
            glLoadIdentity();
            gluOrtho2D( 0.0, 1366.0, 768.0,0.0 );
            
            drawPixels(F.rectangleResponse[0].information,F.rectangleResponse[0].x,F.rectangleResponse[0].y,100,100);

            glutSwapBuffers();
        }
    }    
}


void timer(int x){
    //----------------
    // add functions that chage global objects
    //----------------
    C.askForFrames();
    glutPostRedisplay();

    glutTimerFunc(500,timer,0); // recusive call to update
}
void keyboard(unsigned char c , int x , int y){

    KeyEvent key_pressed;
    
    key_pressed.key = c;
    key_pressed.downFlag = true;
    PointerEvent p;p.isMoved = false;p.isPressed = false;
    C.sendMessage(key_pressed,p);
    
    std::cout << "Keyboard Input :: " + std::to_string(int(c)) << std::endl;

  
    //-------------------
   
} 
void mouse(int button , int state , int x , int y){
    PointerEvent pointer;
    pointer.isMoved = true;
    pointer.isPressed = true;
    pointer.x_position = x;
    pointer.y_position = y;
    KeyEvent k;k.downFlag = false;
    pointer.button = button;
    C.sendMessage(k,pointer);
    std::cout << "Mouse Input :: " + std::to_string(int(button)) << std::endl;
    std::cout << "Mouse Coordinates :: "+std::to_string(x)+","+std::to_string(y) <<std::endl;    
}

void processSpecialKeys(int key, int x, int y) {

    switch(key) {
        case GLUT_KEY_F1 :
            std::cout << "Keyboard Input :: f1" << std::endl;
            C.shutitDown();
            glutLeaveMainLoop();
            break;
        case GLUT_KEY_F2 :
            std::cout << "Keyboard Input :: f2" << std::endl;
            break;
        case GLUT_KEY_F3 :
            std::cout << "Keyboard Input :: f3" << std::endl;
            break;
    }
}

int handler(int argc, char** argv)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(window_width,window_height);
    glutCreateWindow("remoteDesktopApplication"); //creates window with title given 

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouse);
    glutTimerFunc(0,timer,0);
   

    //printf("starting main loop now\n");
    glutMainLoop(); //should call after finishing rendering process
    return 0;
}




