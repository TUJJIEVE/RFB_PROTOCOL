//#include "client.h"
//#include <GL/glew.h> 
//#include <GL/glut.h>
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

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "messages.h"

#include <string.h>
#include <string>
class FrameBuffer{
    public:
    //     //FrameBuffer(char *);
    //     ~FrameBuffer(){
    //         munmap(fbMem,screensize);
    //         close(fbd);
    //     }
    //     FrameBuffer();
    //     void setBuffer(char * fbdevice);
    //     /// fill the information in to fInfo of the region described by the rectangle
    //     void queryBuffer(char *,Rectangle);
    //     /// write the information present to frame buffer of the given regions described by rectangle
    //     void writeBuffer(char *,Rectangle);
         int screensize = 0;
         struct fb_var_screeninfo vinfo;
    // private:
         int fbd = 0;
        
         struct fb_fix_screeninfo finfo;
    //     char * fbMem = 0;
        void setBuffer(char *);    
        Display* display;
        Window root;
        int x,y,width,height;
        XImage* img;
        bool init;
        //struct fb_var_screeninfo vinfo;
        
        void setBuffer2(int, int, int, int);

        void queryBuffer(unsigned char *,Rectangle);

        ~FrameBuffer();


};

#endif