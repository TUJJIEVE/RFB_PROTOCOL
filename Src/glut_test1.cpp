#include <GL/glew.h> 
#include <GL/glut.h>
#include <iostream>
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

void render(void);
void keyboard(unsigned char c , int x , int y);
void mouse(int button , int state , int x , int y);
void timer(int x);
// void pixelmaps(unsigned char * pixelBuffer);
int main(int argc, char** argv)
{
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1366, 768);
    glutCreateWindow("remoteDesktopApplication"); //creates window with title given 

    glutDisplayFunc(render);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0,timer,0);
   
    glutMainLoop(); //should call after finishing rendering process
    return 0;
}

void render(){
    int window_width{1366 } ;
    int window_height{786};
    glClear(GL_COLOR);
    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_width, 0, window_height, -1, 1);
    

    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    int wrap = 1;
    long int screensize = 0;
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }
    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    GLubyte * fbp = nullptr;
    
    fbp = (GLubyte  * ) mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);    
    if(fbp != MAP_FAILED){
        printf("mmap seems worked\n");
    };
    if ((int)*fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        printf("Child errno: %s\n",strerror(errno));
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    int r_mask = (1 << vinfo.red.length) - 1;
    int g_mask = (1 << vinfo.green.length) - 1;
    int b_mask = (1 << vinfo.blue.length) - 1;
       
    int bytes_per_pixel = vinfo.bits_per_pixel / 8;
    if ((vinfo.bits_per_pixel != 16) && (vinfo.bits_per_pixel != 24) && (vinfo.bits_per_pixel != 32))
    {
        printf(" only 16, 24 and 32 ");
        fprintf(stderr, "bits per pixels supported\n");
        exit(EXIT_FAILURE);
    }
    
    unsigned char pixelBuffer[vinfo.xres][vinfo.yres][3];//[1366][768][3]
    for(int y = 0 ; y < vinfo.yres ; y++){
        for(int x = 0 ; x < vinfo.xres ; x++){
            int pb_offset = 3 * x;
            size_t fb_offset = x * (bytes_per_pixel)+ y * finfo.line_length;
            uint32_t pixel = 0;
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
                // nothing to do                
                break;
            }
            unsigned char r = (pixel >> vinfo.red.offset) & r_mask;
            unsigned char g = (pixel >> vinfo.green.offset) & g_mask;
            unsigned char b = (pixel >> vinfo.blue.offset) & b_mask;
            pixelBuffer[x][y][0] =  (r * 0xFF)/r_mask;
            pixelBuffer[x][y][1] = (g * 0xFF)/g_mask;
            pixelBuffer[x][y][2] = (b * 0xFF)/b_mask;                            
        }        
    }
    munmap(fbp, screensize);
    close(fbfd);    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        // glTranslatef(0.5 ,0.5,0.5);
        
        glRasterPos2i(0,0);   
        glDrawPixels(vinfo.yres,vinfo.xres,GL_RGB ,GL_UNSIGNED_BYTE,pixelBuffer);
        // glRotatef(1,0,0,1);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glPopMatrix();
    // 
    // glLoadIdentity();;
    std::cout << "completed file" << std::endl;
    
    glutSwapBuffers();    
}

void timer(int x){
    glutPostRedisplay();
    glutTimerFunc(5000,timer,0); 
}

void keyboard(unsigned char c , int x , int y){
    std::cout << "Keyboard Input :: " + std::to_string(int(c)) << std::endl;
   
}
void mouse(int button , int state , int x , int y){
    std::cout << "Mouse Input :: " + std::to_string(int(button)) << std::endl;
    std::cout << "Mouse Coordinates :: "+std::to_string(x)+","+std::to_string(y) <<std::endl;    
}
/*
void pixelmaps(unsigned char * pixelBuffer){ //*e[ D2 ][ D3 ]
    glClear(GL_COLOR_BUFFER_BIT);
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    int wrap = 1;
    long int screensize = 0;
    fbfd = open("/dev/fb0", O_RDWR);
    // fbfd = open("tttt.raw", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }
    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    GLubyte * fbp = nullptr;
    
    fbp = (GLubyte  * ) mmap(NULL, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);    
    if(fbp != MAP_FAILED){
        printf("mmap seems worked\n");
    };
    if ((int)*fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        printf("Child errno: %s\n",strerror(errno));
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    int r_mask = (1 << vinfo.red.length) - 1;
    int g_mask = (1 << vinfo.green.length) - 1;
    int b_mask = (1 << vinfo.blue.length) - 1;
       
    int bytes_per_pixel = vinfo.bits_per_pixel / 8;
    if ((vinfo.bits_per_pixel != 16) && (vinfo.bits_per_pixel != 24) && (vinfo.bits_per_pixel != 32))
    {
        printf(" only 16, 24 and 32 ");
        fprintf(stderr, "bits per pixels supported\n");
        exit(EXIT_FAILURE);
    }
    
    // unsigned char pixelBuffer[vinfo.xres][vinfo.yres][3];//[1366][768][3]
    for(int y = 0 ; y < vinfo.yres ; y++){
        for(int x = 0 ; x < vinfo.xres ; x++){
            int pb_offset = 3 * x;
            size_t fb_offset = x * (bytes_per_pixel)+ y * finfo.line_length;
            uint32_t pixel = 0;
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
                // nothing to do                
                break;
            }
            unsigned char r = (pixel >> vinfo.red.offset) & r_mask;
            unsigned char g = (pixel >> vinfo.green.offset) & g_mask;
            unsigned char b = (pixel >> vinfo.blue.offset) & b_mask;
            pixelBuffer[(y*vinfo.xres + x)*3 + 0] =  (r * 0xFF)/r_mask;
            pixelBuffer[(y*vinfo.xres + x)*3 + 1] = (g * 0xFF)/g_mask;
            pixelBuffer[(y*vinfo.xres + x)*3 + 2] = (b * 0xFF)/b_mask;                            
            // pixelBuffer[x][y][0] =  (r * 0xFF)/r_mask;
            // pixelBuffer[x][y][1] = (g * 0xFF)/g_mask;
            // pixelBuffer[x][y][2] = (b * 0xFF)/b_mask;                            
        }        
    }
    munmap(fbp, screensize);
    close(fbfd);
    return ;
}
*/