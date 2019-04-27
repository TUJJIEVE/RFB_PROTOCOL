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
#include "threadpool.h"
#include "messages.h"
#include "frameBuffer.h"
#include "cache.h"
#include "io.h"

using namespace std;
#define MAX_REQSIZE 400
#define NUM_THREADS 10
#define SERVER_PORT 5093

typedef std::function<int(int)> serverJob;

class Server{

    public:
        void shutdown();
        Server(int numFreeSocks);
        void initiate();
        int handshake(int,sockaddr_in);
        int frameSending(int); 
        int bufferReceiving(int);
        void listenConnections();
    private:
        std::vector<std::string>  prepareRect(Rectangle *,int);
        ServerInit prepareInit(int);
        
        IO ioDevice;
        Cache frameCaches;
        int numConnected;
        vector<int> activeSockets;
        queue<int> freeSockets;
        int firstClient;
        ThreadPool tPool;
        char *buff ="/dev/fb0";
        FrameBuffer frameBuffer;
        bool isMutliMode;
        sockaddr_in serverAddr;
        int serverListenSock;
        int totalSockets;
        bool isOver = false;
        
};
