#include <linux/fb.h>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
int main(){

    int fb = open("/dev/fb0",O_RDWR);
    if (fb <0){
        printf("Not able to open\n");
    }
    
    fb_var_screeninfo info;
    fb_var_screeninfo finfo;
    ioctl(fb,FBIOGET_VSCREENINFO,&finfo);
    ioctl(fb,FBIOGET_VSCREENINFO,&info);
    
    size_t len = info.xres*4 *info.yres;
    std::cout << info.xres<<std::endl << info.yres <<std::endl;
    std::cout << finfo.xres<<std::endl << finfo.yres <<std::endl;
    
    unsigned * mem =(unsigned *) mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fb,0);
    if (mem == nullptr){
        printf("Unsucessfull\n");
    }
}