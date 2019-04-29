#include <cstdio>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>


class keyboard
{
private:
	std::string filename ;
	std::string  buffer ;
public:	
	keyboard(std::string );
	bool pasteStringOnTtyBuffer(std::string );
	bool pressEnter();
	bool sendChar(char c);
};