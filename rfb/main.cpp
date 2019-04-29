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
// void copyPixels(unsigned char * dest , unsigned char * src , int x , int y , int w , int h , int W . int H){
//     int ii;
//     for(int j = 0 ; j < h ; j++){
//         ii = ((y+j)*W+x)*3;
//         for(int i = 0 ; i < w ; i++){
//             src[ii] = dest[(j*w+i)*3 + 0];
//             src[ii+1] = dest[(j*w+i)*3 + 1];
//             src[ii + 2] = dest[(j*w+i)*3 + 2];
//             ii+=3;
//         }

//     }
// }

void drawPixels(unsigned char * data , int x , int y , int w , int h){
    glBegin (GL_POINTS);
        for (int yy = 0; yy < h; yy++) {
            for (int xx = 0; xx < w; xx++) {
                glColor3f((float)(data[(yy*w+xx)*3+0]) / 255,(float)(data[(yy*w+xx)*3+1]) / 255,(float)(data[(yy*w+xx)*3+2]) / 255);
                glVertex2i( xx + x , yy + y );
                // printf("ploting points\n");
            }
        }
    glEnd ();
    printf("terminated drawPixels\n");
}


void render(){
    printf("rendering\n");
    
    while (!C.frameQs.empty()){
    FrameBufferUpdate F;
    int size = -1;
    {
        std::unique_lock<std::mutex> l(C.m);
        F = C.frameQs.front();
        size = C.frameQs.size();
        C.frameQs.pop();
    }
    if (size>0){
            //glClear(GL_COLOR_BUFFER_BIT);
    
    //unsigned char aa[786*1366*3];
    //screen.getPixels(aa,0,0,1366,786);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 1366.0, 768.0,0.0 );
    //unsigned char bb[100*100*3];
    // copyPixels()
    // memcpy(bb,aa,100*100*3);
    // int ii = 0;
    // for(int j = 0 ; j < 100 ; j++){
    //     for(int i = 0 ; i < 100 ; i++){

    //         bb[(j*100+i)*3 + 0] = aa[(j*1366+i)*3 + 0];
    //          bb[(j*100+i)*3 + 1] = aa[(j*1366+i)*3 + 1];
    //           bb[(j*100+i)*3 + 2] = aa[(j*1366+i)*3 + 2];
    //     }
    // }
    printf("all values are set\n");
    
    drawPixels(F.rectangleResponse[0].information,F.rectangleResponse[0].x,F.rectangleResponse[0].y,100,100);
    // int y = 0 , x = 0 , h = 786 , w = 1366 ;
    //   glBegin (GL_POINTS);
    //     for (int yy = 0; yy < h; y++) {
    //         for (int xx = 0; xx < w; x++) {
    //             glColor3f((float)(aa[(yy*w+xx)*3+0]) / 255,(float)(aa[(yy*w+xx)*3+1]) / 255,(float)(aa[(yy*w+xx)*3+2]) / 255);
    //             glVertex2i( xx + x , yy + y );
    //         }
    //     }
    // glEnd ();
    std::cout << "completed rendering" << std::endl;
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
    int shift_ctrl_alt = glutGetModifiers();
 
    if(shift_ctrl_alt == GLUT_ACTIVE_SHIFT) std::cout << "active : shift" << std::endl;
    else if(shift_ctrl_alt == GLUT_ACTIVE_CTRL) std::cout << "active : ctrl" << std::endl;
    else if(shift_ctrl_alt == GLUT_ACTIVE_ALT)std::cout << "active : alt" << std::endl;
    else if(shift_ctrl_alt == GLUT_ACTIVE_CTRL  | GLUT_ACTIVE_SHIFT) std::cout << "active : ctrl,shift" << std::endl;
    else if(shift_ctrl_alt == GLUT_ACTIVE_ALT | GLUT_ACTIVE_SHIFT)std::cout << "active : alt,shift" << std::endl;
    else if(shift_ctrl_alt == GLUT_ACTIVE_CTRL | GLUT_ACTIVE_ALT) std::cout << "active : ctrl,alt" << std::endl;
    else if(shift_ctrl_alt == GLUT_ACTIVE_ALT | GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL)std::cout << "active : alt,shift,ctrl" << std::endl;
    
    std::cout << "Keyboard Input :: " + std::to_string(int(c)) << std::endl;
    //--------------------

  
    //-------------------
   
} 
void mouse(int button , int state , int x , int y){
    std::cout << "Mouse Input :: " + std::to_string(int(button)) << std::endl;
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
    glutCreateWindow("remoteDesktopApplication"); //creates window with title given 

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(mouse);
    glutTimerFunc(0,timer,0);
   

    printf("starting main loop now\n");
    glutMainLoop(); //should call after finishing rendering process
    return 0;
}




