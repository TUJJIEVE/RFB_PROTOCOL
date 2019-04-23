#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "messages.h"
#include <string.h>
class FrameBuffer{
    public:
        FrameBuffer(char *);
        ~FrameBuffer(){
            munmap(fbMem,screensize);
            close(fbd);
        }
        /// fill the information in to fInfo of the region described by the rectangle
        void queryBuffer(char *,Rectangle);
        /// write the information present to frame buffer of the given regions described by rectangle
        void writeBuffer(char *,Rectangle);

    private:
        int fbd = 0;
        struct fb_var_screeninfo vinfo;
        struct fb_fix_screeninfo finfo;
        char * fbMem = 0;
        int screensize = 0;

};