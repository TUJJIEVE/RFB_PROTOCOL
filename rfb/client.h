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
#include <math.h>
#include <queue>
#define CLIENT_PORT 5094

class Client{

    public:
        void initiateConnection();
        void bufferRequest(int,int,int);
        void receiveBuffer();
        void sendMessage(KeyEvent ,PointerEvent );
        void setupFrameConnection();
        void prepareRectangles(FrameBufferUpdateRequest *,int,int);
        void handleGUIevents(); // function to handle gui
        void setGUI(GUI *);
        void askForFrames();
        void shutitDown();
        std::queue<FrameBufferUpdate> frameQs;
        Client();
        bool shutDown;
        std::mutex m;
    private:
        int clientSocket;
        std::vector<std::thread> workers;
        int frameSocket;
        int frameBufferSocket;
        sockaddr_in serverAddr;
        sockaddr_in clientAddr;
        PixelFormat pixie;
        int screen_xres;
        int screen_yres;
        int maxSize;
        int isControl;
        GUI* guiWindow;
};