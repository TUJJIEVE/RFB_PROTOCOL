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

ServerInit Server::prepareInit(int sock){
    ServerInit initMessage;
    if (sock == firstClient){
        // give him the control rights
        
        initMessage.isControl = true;
        // set parameters for the message

    }
    else{
        // give the client only screen sharing rights
        initMessage.isControl = false;
        // set the parameters of the message
    }
}

void Server::prepareRect(Rectangle &rect){
    // read frame buffer and then fill the info into the rectangle object
    Rectangle * temp = frameCaches.lookup(rect);
    if (temp == nullptr){
        // not present in the cache
        // fill in the info 

    }
    else{
        auto rectangle = *temp;
        strcpy(rect.info,rectangle.info);

    }


}

int Server::frameSending(int connectedSock){
    int recvCount = 0;
    ClientMessage cliMessage;
    fcntl(connectedSock,F_SETFL,O_NONBLOCK);
    while (true){
        recvCount = recv(connectedSock,&cliMessage,sizeof(cliMessage),0);
        
        if (recvCount>0){
            
            if (cliMessage.isShuttingDown){
                // start shutting sequence.
            
                break;
            }

            if (cliMessage.key.downFlag){
                // key event happened

            }
            else if (cliMessage.pointer.isMoved || cliMessage.pointer.isPressed){
                // Mouse poiner event happened

            }

            else{
                if (cliMessage.request.incremental){
                    /// send on lt if it's changed
                
                }
                else{
                    // send the complete info 
                    Rectangle queriedRect(cliMessage.request.x_position,cliMessage.request.y_position,cliMessage.request.width,cliMessage.request.height);
                    prepareRect(queriedRect);
                    // Make a caching mechanism to store the already asked 
                    FrameBufferUpdate response(1);
                    response.responseRect = queriedRect;
                    // send the responseRect
                }
            }
        }

    }





}

int Server::handshake(int connectedSock){
    // Initial handshake messages to take place here
    // If it's not the first client then tell the client you are only screen sharing
    
    ServerInit initMessage = prepareInit(connectedSock);

    int sendcount = 0;
    while(sendcount<=0){
        sendcount = send(connectedSock,(void *)&initMessage,sizeof(initMessage));
    }

    serverJob frameJob = std::bind(&Server::frameSending,this,connectedSock);
    pair<serverJob,int> job = make_pair(frameJob,connectedSock);
    tPool.addJobs(1,job);

    return 0;

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

