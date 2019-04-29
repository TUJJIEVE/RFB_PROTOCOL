#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <sys/mman.h>
#include <cstring>
#include <errno.h>


//assuming 1366 x 786 screen window only
const int wwidth = 1366 ; 
const int wheight = 786 ;

class PixelBuffer
{
private:	
	std::string filename;
public:
	unsigned char pixelBuffer[wheight][wwidth][3] ;
	PixelBuffer(std::string f);
	void updatePixels(); //updates pixel values on buffer based on file
	std::string getFilename();
};