#include "keyboard.h"

keyboard::keyboard(std::string s = "dev/tty3"){
	this->filename = s ;
}

bool keyboard::pasteStringOnTtyBuffer(std::string t){
	char * text = (char * )t.c_str();
	int fd = open((char *)filename.c_str(), O_RDONLY);
	if (fd < 0) {
		printf("error:unable to open file\n");
		//----------------------------
		// send  back message  to client the could not open /dev/tty3 
		//---------------------------
		return 1;
	}	
	for (int i = 0; i < t.length() ; i++)
	if (ioctl(fd, TIOCSTI, text+i)) {
		printf("unable to send char\n");
		//------------------------------
		// send msg that could not paste string
		// -------------------------------
		return 1;
	}
	return 0;
}
bool keyboard::pressEnter(){
	return pasteStringOnTtyBuffer("\n");
}
bool keyboard::sendChar(char c){
	int fd = open((char *)filename.c_str(), O_RDONLY);
	if (fd < 0) {
		printf("error:unable to open file\n");
		//----------------------------
		// send  back message  to client the could not open /dev/tty3 
		//---------------------------
		return 1;
	}	
	if (ioctl(fd, TIOCSTI,&c)) {
		printf("unable to send char\n");
		//------------------------------
		// send msg that could not paste char
		// -------------------------------
		return 1;
	}
	return 0;
}

