#include "client.h"
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

void render();
void timer(int x);
void keyboard(unsigned char c , int x , int y);
void mouse(int button , int state , int x , int y);
int handler(int argc, char** argv);
void processSpecialKeys(int key, int x, int y);

Client C(nullptr);

int main(int argc, char** argv){
    
    
	handler(argc , argv) ; 

	return 0;
}

 
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
   
    glutMainLoop(); //should call after finishing rendering process
    return 0;
}


