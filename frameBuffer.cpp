#include "frameBuffer.h"


void FrameBuffer::setBuffer(char * fbDevice){
    fbd = open(fbDevice,O_RDWR);
    if (fbd == -1){
        printf("Error: Cannot open the frame buffer object");
        exit(2);
    } 

    if (ioctl(fbd,FBIOGET_FSCREENINFO,&finfo)){
        perror("ERROR reading fixed information\n");
        exit(2);
    }
    if (ioctl(fbd,FBIOGET_VSCREENINFO,&vinfo)){
        perror("Error reading variable screen information\n");
        exit(2);
    }
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    fbMem = (char *) mmap(0,screensize,PROT_READ|PROT_WRITE,MAP_SHARED,fbd,0);
    printf("The frame buffer device was mapped successfully\n");

} 


void FrameBuffer::queryBuffer(char * fInfo,Rectangle rectangle){
    long int location = 0;
    int offset = 0;
    for (int y = rectangle.y_position;y<rectangle.y_position+rectangle.height;y++){
        for (int x = rectangle.x_position;x<rectangle.x_position+rectangle.width;x++){
            
            location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) + 
                        (y + vinfo.yoffset) * (finfo.line_length);
        
            strcpy(fInfo+offset,(fbMem+location));
            offset = sizeof(*(fbMem+location));

        }
    }    
    return;

}

void FrameBuffer::writeBuffer(char * fInfo,Rectangle rectangle){
    long int location = 0;
    int offset = 0;

    for (int y=rectangle.y_position;y<rectangle.y_position+rectangle.height;y++){
        for (int x = rectangle.x_position;x<rectangle.x_position+rectangle.width;x++){
            location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) + 
                        (y + vinfo.yoffset) * (finfo.line_length);
        
            strcpy((fbMem+location),fInfo+offset);
            offset = sizeof(*(fbMem+location));
        }
    }

    return;
}

