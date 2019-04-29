#include <GL/glew.h> 
#include <GL/glut.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <vector>
#include "PixelBuffer.h"

const int window_width{1366 } ;
const int window_height{786};



 
void render(){

    glClear(GL_COLOR);
    
    
    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,window_width,0,window_width,1,-1);

    PixelBuffer p("/dev/fb0") ; //replace with the file that contains pixelmaps
    if(p.updatePixels() != 0){
        printf("error :: while updatePixels\n");
    }
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glRasterPos2i(0,0);          
        glDrawPixels(window_width,window_height,GL_RGB ,GL_UNSIGNED_BYTE,p.pixelBuffer);       
    glPopMatrix();

    // glLoadIdentity();;
    std::cout << "completed rendering" << std::endl;
    
    glutSwapBuffers();    
}


void timer(int x){
    //----------------
    // add functions that chage global objects
    //----------------
    glutPostRedisplay();
    glutTimerFunc(5000,timer,0); // recusive call to update
}
void keyboard(unsigned char c , int x , int y){
    std::cout << "Keyboard Input :: " + std::to_string(int(c)) << std::endl;
   
} 
void mouse(int button , int state , int x , int y){
    std::cout << "Mouse Input :: " + std::to_string(int(button)) << std::endl;
    std::cout << "Mouse Coordinates :: "+std::to_string(x)+","+std::to_string(y) <<std::endl;    
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
    glutMouseFunc(mouse);
    glutTimerFunc(0,timer,0);
   
    glutMainLoop(); //should call after finishing rendering process
    return 0;
}


