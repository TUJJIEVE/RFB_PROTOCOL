#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <fcntl.h>
#include <chrono>  
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <thread>
#include <fstream>
#include <vector>
#include "messages.h"
#include "gui.h"

#define CLIENT_PORT 5094

class Client{

    public:
        void initiateConnection();
        void bufferRequest(int);
        void receiveBuffer();
        void sendMessage();
        void setupFrameConnection();
        void handleGUIevents(); // function to handle gui
        Client(GUI *);

    private:
        int clientSocket;
        std::vector<std::thread> workers;
        int frameSocket;
        sockaddr_in serverAddr;
        sockaddr_in clientAddr;
        int bitsPerPixel;
        int screen_xres;
        int screen_yres;
        int maxSize;
        int isControl;
        GUI* guiWindow;
};