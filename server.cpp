#include "server.h"



Server::Server(int freeSocks){

    frameBuffer.setBuffer("/dev/fb0");
    serverListenSock = socket(AF_INET,SOCK_STREAM,0);
    if (serverListenSock == -1){
        printf("Socket not created\n");
        exit(EXIT_FAILURE);
    }
    printf("listen socket craeted\n");
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);
    tPool.start(10);
    totalSockets = freeSocks;

    if (bind (serverListenSock,(const sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        printf("Binding Failed");
        exit(EXIT_FAILURE);

    }
    printf("Binding socket is successfull\n");
    listen(serverListenSock,200);
    printf("creating free sockets\n");
    for (int i=0;i<freeSocks;i++){
        int fd = socket(AF_INET,SOCK_STREAM,0);
        freeSockets.push(fd);
    }   
    printf("creation successfull\n");
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
    initMessage.pf.red_shift = frameBuffer.vinfo.red.offset;
    initMessage.pf.blue_shift = frameBuffer.vinfo.blue.offset;
    initMessage.pf.green_shift = frameBuffer.vinfo.green.offset;
    initMessage.pf.redMax = frameBuffer.vinfo.red.length;
    initMessage.pf.greenMax = frameBuffer.vinfo.green.length;
    initMessage.pf.blueMax  =frameBuffer.vinfo.blue.length;
    return initMessage;

}

void Server::prepareRect(Rectangle *rect,int numRectangles,uint8_t * buff){
    // read frame buffer and then fill the info into the rectangle object
    std::vector<std::string> response;
    for (int i=0;i<numRectangles;i++){
        //printf("%d %d %d %d\n",rect[i].width,rect[i].height,rect[i].x_position,rect[i].y_position);
        // if possible implement caching mechanism
        // now prepare the rectangles and fill the into the resonse array        
        int size = rect[i].width * rect[i].height * frameBuffer.vinfo.bits_per_pixel/8;
        //printf("Size is %d\n",size);
        //unsigned char buff[size];
        frameBuffer.queryBuffer(buff,rect[i]);
        //std::string temp = buff;
        //response.push_back(temp);
        // for (int i=0;i<40000;i++){
        //     printf("%c",buff[i]);
        // }
    }
    //printf("frame sizes %d\n",response.size());
    //return response;
}
// Opens another connection just for listening to frame buffer update requests
int Server::bufferReceiving(int frameSocket){
    int recvCount = 0;
    int sendCount = 0;
    FrameBufferUpdateRequest cliMessage;
    //printf("receiving\n");
    fcntl(frameSocket,F_SETFL,O_NONBLOCK);
    while (true){
        //printf("recv\n");
            recvCount = recv(frameSocket,&cliMessage,sizeof(cliMessage),0);
            
        //recvCount = recv(frameSocket,&cliMessage,sizeof(cliMessage),0);
        
        if (recvCount>0){
            //printf("recv count %d %d %d\n",recvCount,cliMessage.isShuttingDown,cliMessage.numRectangles);            
            if (cliMessage.isShuttingDown){
                // start shutting sequence.
                printf("shutting down\n");
                break;
            }

            else{
                if (cliMessage.incremental){
                    /// send on lt if it's changed
                
                }
                else{
                    //printf("preparing\n");
                    // send the complete info 
                    //Rectangle queriedRect(cliMessage.request.x_position,cliMessage.request.y_position,cliMessage.request.width,cliMessage.request.height);

                    //ServerMessage servResponse;//(cliMessage.numRectangles);
                    //servResponse.buffUpdate.numRectangles = cliMessage.numRectangles;
                    //printf("The number of rectangles to server %d\n",servResponse.buffUpdate.numRectangles);
                    uint8_t frameInfo[40000];
                    //std::vector<std::string> frameInfo = prepareRect(cliMessage.rectangleRequests,cliMessage.numRectangles);
                    prepareRect(cliMessage.rectangleRequests,cliMessage.numRectangles,frameInfo);                    
                    //printf("Created frame buffers\n");
                    // Make a caching mechanism to store the already asked 
                    FrameBufferUpdate response;//(cliMessage.numRectangles);
                    response.numRectangles = cliMessage.numRectangles;
                    for (int i =0;i<response.numRectangles;i++){
                        for (int j=0;j<40000;j++){
                            response.rectangleResponse[i].information[j] = frameInfo[j];
                        }
                        //strcpy(servResponse.buffUpdate.rectangleResponse[i].information,frameInfo);
                    }
                    response.rectangleResponse[0].x = cliMessage.rectangleRequests[0].x_position;
                    response.rectangleResponse[0].y = cliMessage.rectangleRequests[0].y_position;
                    //printf("Sending response\n");

                    // send the responseRect use while loop to ensure complete sending of packets
                    int totalSend = 0;
                    int sendCount = 0;
                    //printf("%d\n",sizeof(response));
                    // for (int i=0;i<40000;i++){
                    //     printf("%c",response.rectangleResponse[0].information[i]);
                    // }
                    while (totalSend < sizeof(response)){
                        //printf("sending\n");
                        sendCount = send(frameSocket,((char *)&response) +(4*totalSend),sizeof(response) - totalSend,0 );
                        if (sendCount >0) totalSend += sendCount;
                        //printf("Send %d total %d\n",sendCount,totalSend);
                    }
                    //sendCount = send(frameSocket,&response,sizeof(response),0);
                    //printf("sendcount %d %d\n",sendCount,sizeof(response));
                    //printf("Buffer response done\n");
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
            printf("Received key or pointer event\n");
            if (cliMessage.isShuttingDown){
                // start shutting sequence.
            
                break;
            }
            
            if (cliMessage.key.downFlag){
                printf("key event happened\n");
                // key event happened
                ioDevice.keyPress_release(cliMessage.key.key);
            }
            else if (cliMessage.pointer.isMoved || cliMessage.pointer.isPressed){
                // Mouse poiner event happened
                printf("mouse event happened\n");
                if (cliMessage.pointer.isMoved){
                    ioDevice.moveMouse(cliMessage.pointer.x_position,cliMessage.pointer.y_position);
                }
                if (cliMessage.pointer.isPressed){
                    ioDevice.mouseClick(cliMessage.pointer.button);
                }
            }   

        }

    }

}

int Server::handshake(int connectedSock,sockaddr_in clientAddr){
    // Initial handshake messages to take place here
    // If it's not the first client then tell the client you are only screen sharing
    int frameSocket = socket(AF_INET,SOCK_STREAM,0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    sockaddr_in frameClient;
    frameClient.sin_addr.s_addr = clientAddr.sin_addr.s_addr;
    frameClient.sin_family = clientAddr.sin_family;
    frameClient.sin_port = htons(SERVER_PORT + 1);
    printf("socket %d port %d\n",connectedSock,clientAddr.sin_port);
    printf("socket %d port %d\n",connectedSock,frameClient.sin_port);

    
    if (connect(frameSocket,(sockaddr*)&frameClient,sizeof(frameClient)) <0) {
        printf("second connection not able to open\n");
        return -1;
    }

    printf("Second connection established\n");
    ServerInit initMessage = prepareInit(connectedSock);
    
    int sendcount = 0;
    while(sendcount<=0){
        sendcount = send(connectedSock,(void *)&initMessage,sizeof(initMessage),0);
    }
    printf("init %d send %d\n",sizeof(initMessage),sendcount);
    serverJob bufferReqJob = std::bind(&Server::bufferReceiving,this,frameSocket);
    serverJob frameJob = std::bind(&Server::frameSending,this,connectedSock);
    tPool.addJobs(2,frameJob,bufferReqJob);

    return 0;

}

void Server::listenConnections(){
    //fcntl(serverListenSock,F_SETFL,O_NONBLOCK);
    printf("Listening for connections\n");
    while(true){

        sockaddr_in clientAddr;
        int len = sizeof(clientAddr);
        int clientSock = accept(serverListenSock,(sockaddr*) &clientAddr,(socklen_t*)&len);

        if (clientSock !=-1){
                printf("connection came at socket %d %d\n",clientSock,clientAddr.sin_port);
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

