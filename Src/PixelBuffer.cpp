#include "PixelBuffer.h"

PixelBuffer::PixelBuffer(std::string s){
    this->filename = s;
    updatePixels();
}
void PixelBuffer::updatePixels(){
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    
    long int screensize = 0;
    fbfd = open(filename.c_str(), O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        return ;
    }
    printf("The framebuffer device was opened successfully.\n");
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        return ;
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        return ;
    }
    // printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    unsigned char * fbp = nullptr;
    fbp = (unsigned char  * ) mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);    
    if(fbp == MAP_FAILED){
        perror("mmap didnot worked\n");
        return ;
    };

    printf("The framebuffer device was mapped to memory successfully.\n");

    int r_mask = (1 << vinfo.red.length) - 1; 
    int g_mask = (1 << vinfo.green.length) - 1;
    int b_mask = (1 << vinfo.blue.length) - 1;       
    int bytes_per_pixel = vinfo.bits_per_pixel / 8;


    if ((vinfo.bits_per_pixel != 16) && (vinfo.bits_per_pixel != 24) && (vinfo.bits_per_pixel != 32))
    {
        perror(" only 16, 24 and 32 bits per pixel are supported");
        return ;
    }
    for(int y = 0 ; y < vinfo.yres ; y++){
        for(int x = 0 ; x < vinfo.xres ; x++){
            int pb_offset = 3 * x;
            size_t fb_offset = (vinfo.xres - 1 - x) * (bytes_per_pixel)+ y * finfo.line_length;
            unsigned int pixel = 0;
            switch (vinfo.bits_per_pixel)
            {
            case 16: 
                pixel = *((uint16_t *)(fbp + fb_offset));
                break;   
            case 24:
                pixel += *(fbp + fb_offset);
                pixel += *(fbp + fb_offset + 1) << 8;
                pixel += *(fbp + fb_offset + 2) << 16;
                break;
            case 32:
                pixel = *((uint32_t *)(fbp + fb_offset));
                break;
            default:
                // will not come here             
                break;
            }
            unsigned char r = (pixel >> vinfo.red.offset) & r_mask;
            unsigned char g = (pixel >> vinfo.green.offset) & g_mask;
            unsigned char b = (pixel >> vinfo.blue.offset) & b_mask;                         
            pixelBuffer[y][x][0] =  (r * 0xFF)/r_mask;
            pixelBuffer[y][x][1] = (g * 0xFF)/g_mask;
            pixelBuffer[y][x][2] = (b * 0xFF)/b_mask;                            
        }        
    }
    return ;
}

std::string PixelBuffer::getFilename(){
    return filename;
}