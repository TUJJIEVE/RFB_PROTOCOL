#include "server.h"



Server::Server(int freeSocks){
    frameBuffer.setBuffer("/dev/fb0");
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
    if (sock == firstClient) initMessage.isControl = true;
    else initMessage.isControl = false;
    
    initMessage.pf.bitsPerPix = frameBuffer.vinfo.bits_per_pixel;
    initMessage.bufferHeight = frameBuffer.vinfo.yres;
    initMessage.bufferWidth = frameBuffer.vinfo.xres;
    initMessage.maxReqSize = MAX_REQSIZE;
    return initMessage;

}

std::vector<std::string> Server::prepareRect(Rectangle *rect,int numRectangles){
    // read frame buffer and then fill the info into the rectangle object
    std::vector<std::string> response;
    for (int i=0;i<numRectangles;i++){
        // if possible implement caching mechanism
        // now prepare the rectangles and fill the into the resonse array        
        int size = rect[i].width * rect[i].height * frameBuffer.vinfo.bits_per_pixel/8;
        char buff[size];
        frameBuffer.queryBuffer(buff,rect[i]);
        std::string temp = buff;
        response.push_back(temp);

    }

    return response;
}
// Opens another connection just for listening to frame buffer update requests
int Server::bufferReceiving(int frameSocket){
    int recvCount = 0;
    int sendCount = 0;
    FrameBufferUpdateRequest cliMessage;
    
    fcntl(frameSocket,F_SETFL,O_NONBLOCK);
    while (true){
        recvCount = recv(frameSocket,&cliMessage,sizeof(cliMessage),0);
        
        if (recvCount>0){
            
            if (cliMessage.isShuttingDown){
                // start shutting sequence.
            
                break;
            }

            else{
                if (cliMessage.incremental){
                    /// send on lt if it's changed
                
                }
                else{
                    // send the complete info 
                    //Rectangle queriedRect(cliMessage.request.x_position,cliMessage.request.y_position,cliMessage.request.width,cliMessage.request.height);
                    ServerMessage servResponse;//(cliMessage.numRectangles);
                    servResponse.buffUpdate.numRectangles = cliMessage.numRectangles;
                    std::vector<std::string> frameInfo = prepareRect(cliMessage.rectangleRequests,cliMessage.numRectangles);
                    // Make a caching mechanism to store the already asked 
                    FrameBufferUpdate response;//(cliMessage.numRectangles);
                    response.numRectangles = cliMessage.numRectangles;
                    for (int i =0;i<servResponse.buffUpdate.numRectangles;i++){
                        //servResponse.buffUpdate.rectangleResponse[i].information = (char *) malloc(frameInfo.at(i).size());
                        strcpy(servResponse.buffUpdate.rectangleResponse[i].information,frameInfo.at(i).c_str());
                    }
                    // send the responseRect use while loop to ensure complete sending of packets
                    sendCount = send(frameSocket,&response,sizeof(response),0);

                }
            }
        }

    }  
}

int Server::frameSending(int connectedSock){
    int recvCount = 0;
    int sendcount = 0;
    CliMessage cliMessage;  
  
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
                ioDevice.keyPress_release(cliMessage.key.key);
            }
            else if (cliMessage.pointer.isMoved || cliMessage.pointer.isPressed){
                // Mouse poiner event happened
                if (cliMessage.pointer.isMoved){
                    ioDevice.moveMouse(cliMessage.pointer.x_position,cliMessage.pointer.y_position);
                }
                if (cliMessage.pointer.isPressed){
                    ioDevice.mouseClick(cliMessage.pointer.buttonMask);
                }
            }   

        }

    }

}

int Server::handshake(int connectedSock,sockaddr_in clientAddr){
    // Initial handshake messages to take place here
    // If it's not the first client then tell the client you are only screen sharing
    int frameSocket = socket(AF_INET,SOCK_STREAM,0);
    if (connect(frameSocket,(sockaddr*)&clientAddr,sizeof(clientAddr)) <0) {
        printf("second connection not able to open\n");
    }


    ServerInit initMessage = prepareInit(connectedSock);

    int sendcount = 0;
    while(sendcount<=0){
        sendcount = send(connectedSock,(void *)&initMessage,sizeof(initMessage),0);
    }
    serverJob bufferReqJob = std::bind(&Server::bufferReceiving,this,frameSocket);
    serverJob frameJob = std::bind(&Server::frameSending,this,connectedSock);
    tPool.addJobs(2,frameJob,bufferReqJob);

    return 0;

}

void Server::listenConnections(){
    fcntl(serverListenSock,F_SETFL,O_NONBLOCK);

    while(true){

        sockaddr_in clientAddr;
        int len = sizeof(clientAddr);

        int clientSock = accept(serverListenSock,(sockaddr*) &clientAddr,(socklen_t*)&len);

        if (clientSock !=-1){
                serverJob handshakeJob = std::bind(&Server::handshake,this,clientSock,clientAddr);
                tPool.addJobs(1,handshakeJob);                  
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
    while (!freeSockets.empty()){
        int soc = freeSockets.front();
        close(soc);
        freeSockets.pop();
        
    }

    tPool.stop();

}

