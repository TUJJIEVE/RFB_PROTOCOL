#include "client.h"
#include <GL/glew.h> 
#include <GL/glut.h>
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
//#include "./Src/PixelBuffer.h"
//#include "clientAuthWindow.h"

class PixelBuffer
{
public:
    Display* display;
    Window root;
    int x,y,width,height;
    XImage* img;
    bool init;


    PixelBuffer(int x, int y, int width, int height)
    {
        this->x = x ;
        this->y = y;
        this->width = width ;
        this->height = height ;
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
        init = true;
    }

    void getPixels(unsigned char * data )
    {
         if(init == true)
            init = false;
        else{
            XDestroyImage(img);            
        }        
        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
        int x, y;
        int ii = 0;
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                unsigned long pixel = XGetPixel(img,x,y);
                data[ii + 2]  = (pixel & img->blue_mask);
                data[ii + 1] = (pixel & img->green_mask) >> 8;
                data[ii + 0]  = (pixel & img->red_mask) >> 16;
                ii += 3;
            }
        }  
    }

    ~PixelBuffer()
    {
        if(init == false)
            XDestroyImage(img);
        XCloseDisplay(display);
    }    
};


const int window_width{1366 } ;
const int window_height{786};

void render();
void timer(int x);
void keyboard(unsigned char c , int x , int y);
void mouse(int button , int state , int x , int y);
int handler(int argc, char** argv);
void processSpecialKeys(int key, int x, int y);

// Client C();
PixelBuffer screen(0,0,1366,768);
Client C;

int main(int argc, char** argv){
    
//    if(openingWindow(argc,argv) == false){ 
//         printf("unauthorised client\n" );
//         return 1;
//     } 

    
    printf("successfull authorization \n");
    printf("create connection\n");
    //create connection
    printf("connection created successfully\n");
    handler(argc , argv) ; 
    return 0;
}

 
void render(){

    glClear(GL_COLOR_BUFFER_BIT);
    
    unsigned char aa[786*1366*3];
    screen.getPixels(aa);
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 1366.0, 768.0,0.0 );
    glBegin (GL_POINTS);
        for (int y = 0; y < 786; y++) {
            for (int x = 0; x < 1366; x++) {
                glColor3f((float)(aa[(y*1366+x)*3+0]) / 255,(float)(aa[(y*1366+x)*3+1]) / 255,(float)(aa[(y*1366+x)*3+2]) / 255);
                glVertex2i(x,y);
            }
        }
    glEnd ();

    //std::cout << "completed rendering" << std::endl;
    glutSwapBuffers();    
}


void timer(int x){
    //----------------
    // add functions that chage global objects
    //----------------
    glutPostRedisplay();
    glutTimerFunc(20,timer,0); // recusive call to update
}
void keyboard(unsigned char c , int x , int y){
    // int shift_ctrl_alt = glutGetModifiers();
 
    // if(shift_ctrl_alt == GLUT_ACTIVE_SHIFT) std::cout << "active : shift" << std::endl;
    // else if(shift_ctrl_alt == GLUT_ACTIVE_CTRL) std::cout << "active : ctrl" << std::endl;
    // else if(shift_ctrl_alt == GLUT_ACTIVE_ALT)std::cout << "active : alt" << std::endl;
    // else if(shift_ctrl_alt == GLUT_ACTIVE_CTRL  | GLUT_ACTIVE_SHIFT) std::cout << "active : ctrl,shift" << std::endl;
    // else if(shift_ctrl_alt == GLUT_ACTIVE_ALT | GLUT_ACTIVE_SHIFT)std::cout << "active : alt,shift" << std::endl;
    // else if(shift_ctrl_alt == GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT) std::cout << "active : ctrl,alt" << std::endl;
    // else if(shift_ctrl_alt == GLUT_ACTIVE_ALT | GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL)std::cout << "active : alt,shift,ctrl" << std::endl;
    KeyEvent key_pressed;
    
    key_pressed.key = c;
    key_pressed.downFlag = true;
    PointerEvent p;p.isMoved = false;p.isPressed = false;
    C.sendMessage(key_pressed,p);
    
    std::cout << "Keyboard Input :: " + std::to_string(int(c)) << std::endl;
    //--------------------

  
    //-------------------
   
} 
void mouse(int butto , int state , int x , int y){
    PointerEvent pointer;
    pointer.isMoved = true;
    pointer.isPressed = true;
    pointer.x_position = x;
    pointer.y_position = y;
    KeyEvent k;k.downFlag = false;
    pointer.button = butto;
    C.sendMessage(k,pointer);
    std::cout << "Mouse Input :: " + std::to_string(int(butto)) << std::endl;
    std::cout << "Mouse Coordinates :: "+std::to_string(x)+","+std::to_string(y) <<std::endl;    
}

void processSpecialKeys(int key, int x, int y) {

    switch(key) {
        case GLUT_KEY_F1 :
            std::cout << "Keyboard Input :: f1" << std::endl;
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
    glutCreateWindow("RFB"); //creates window with title given 

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouse);
    glutTimerFunc(0,timer,0);
   
    glutMainLoop(); //should call after finishing rendering process
    return 0;
}