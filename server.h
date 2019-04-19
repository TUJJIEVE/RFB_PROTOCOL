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
#include "cache.h"
using namespace std;

#define NUM_THREADS 10
#define SERVER_PORT 5093

typedef std::function<int(int)> serverJob;

class Server{

    public:
        void shutdown();
        Server(int numFreeSocks);
        void initiate();
        int handshake(int);
        int frameSending(int); 


    private:
        void prepareRect(Rectangle &);
        ServerInit prepareInit(int);
        void listenConnections();
        Cache frameCaches;
        int numConnected;
        vector<int> activeSockets;
        queue<int> freeSockets;
        int firstClient;
        ThreadPool tPool;
        bool isMutliMode;
        sockaddr_in serverAddr;
        int serverListenSock;
        int totalSockets;
        bool isOver = false;
        // 
};
