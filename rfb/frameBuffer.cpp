#include "frameBuffer.h"

// FrameBuffer::FrameBuffer(){

// }

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
    setBuffer2(0,0,vinfo.xres,vinfo.yres);
    //fbMem = (char *) mmap(0,screensize,PROT_READ|PROT_WRITE,MAP_SHARED,fbd,0);
    //printf("The frame buffer device was mapped successfully\n");

} 


// void FrameBuffer::queryBuffer(char * fInfo,Rectangle rectangle){
//     long int location = 0;
//     int offset = 0;
//     for (int y = rectangle.y_position;y<rectangle.y_position+rectangle.height;y++){
//         for (int x = rectangle.x_position;x<rectangle.x_position+rectangle.width;x++){
            
//             location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) + 
//                         (y + vinfo.yoffset) * (finfo.line_length);
        
//             strcpy(fInfo+offset,(fbMem+location));
//             offset = sizeof(*(fbMem+location));

//         }
//     }    
//     return;

// }

// void FrameBuffer::writeBuffer(char * fInfo,Rectangle rectangle){
//     long int location = 0;
//     int offset = 0;

//     for (int y=rectangle.y_position;y<rectangle.y_position+rectangle.height;y++){
//         for (int x = rectangle.x_position;x<rectangle.x_position+rectangle.width;x++){
//             location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel/8) + 
//                         (y + vinfo.yoffset) * (finfo.line_length);
        
//             strcpy((fbMem+location),fInfo+offset);
//             offset = sizeof(*(fbMem+location));
//         }
//     }

//     return;
// }

void FrameBuffer::setBuffer2(int x, int y, int width, int height)
{
    printf("setting buffer %d %d %d %d",x,y,width,height);
    this->x = x ;
    this->y = y;
    this->width = width ;
    this->height = height ;
    display = XOpenDisplay(nullptr);
    root = DefaultRootWindow(display);
    init = true;
}

void FrameBuffer::queryBuffer(unsigned char * data,Rectangle rect )

 {
        //printf("entered getPixels\n");
        if(init == false){

            XDestroyImage(img);
            img = nullptr;
            init = true;
        }  
        while(1){
            img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
            if(img == nullptr){
                printf("error while capturing screen\n");

            }else{
                init = false ;
                break;
            }
        } 
        
        int i, j;
        int ii = 0;
        for (j = 0; j < rect.height; j++) {
            for (i = 0; i < rect.width; i++) {
                //printf("taking pixel val %d %d %d %d \n",i,j,rect.width,rect.height);
                unsigned long pixel = XGetPixel(img,i+rect.x_position,j+rect.y_position);
                data[ii + 2]  = ((pixel) & (img->blue_mask));
                data[ii + 1] = ((pixel) & (img->green_mask)) >> 8;
                data[ii + 0]  = ((pixel) & (img->red_mask)) >> 16;
                ii += 3;
            }
        }  
        //printf("terminated getPixels\n");
}

FrameBuffer::~FrameBuffer()
{
    if(init == false)
        XDestroyImage(img);
    XCloseDisplay(display);
}    
