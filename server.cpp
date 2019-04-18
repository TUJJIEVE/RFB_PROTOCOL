#include "server.h"



Server::Server(int freeSocks){

    serverListenSock = socket(AF_INET,SOCK_STREAM,0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    totalSockets = freeSocks;

    if (bind (serverListenSock,(const sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        printf("Binding Failed");
        exit(EXIT_FAILURE);
    }
    
    listen(serverListenSock,200);
    for (int i=0;i<freeSocks;i++){
        int fd = socket(AF_INET,SOCK_STREAM,0);
        freeSockets.push(fd);
    }   

    isMutliMode = false;
    firstClient = -1;
    activeSockets.reserve(totalSockets);
    numConnected = 0;

}

int Server::handshake(int connectedSock){
    // Initial handshake messages to take place here
    // If it's not the first client then tell the client you are only screen sharing
    
    



}

void Server::listenConnections(){
    fcntl(serverListenSock,F_SETFL,O_NONBLOCK);

    while(true){

        sockaddr_in clientAddr;
        int len = sizeof(clientAddr);

        int clientSock = accept(serverListenSock,(sockaddr*) &clientAddr,(socklen_t*)&len);

        if (clientSock !=-1){
                serverJob handshakeJob = std::bind(&Server::handshake,this,clientSock);
                tPool.addJobs(1,make_pair(handshakeJob,clientSock));                  
                firstClient = clientSock;
                numConnected += 1;
                if (numConnected >1){
                    isMutliMode = true;
                }
        }

        if (isOver){
            return;
        }

    }

}


void Server::initiate(){

    tPool.start(NUM_THREADS);

}

void Server::shutdown(){

    isOver = true;
    for (int i=0;i<activeSockets.size();i++){
        close(activeSockets.at(i));
    }
    while (!freeSockets.empty){
        int soc = freeSockets.front();
        close(soc);
        freeSockets.pop();
        
    }

    tPool.stop();

}

