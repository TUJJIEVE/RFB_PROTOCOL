#include "client.h"

// To Do : ask frame buffers in a patter
// check for key and mouse events

Client::Client(GUI * gui){
    guiWindow = gui;
    clientSocket = socket(AF_INET,SOCK_STREAM,0);
    frameSocket = socket(AF_INET,SOCK_STREAM,0);
    if (clientSocket == -1) printf("Not able to create socket\n");
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = 5093;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = CLIENT_PORT;
    clientAddr.sin_addr.s_addr  = htonl(INADDR_ANY);

    if (bind(frameSocket,(sockaddr*)&clientAddr,sizeof(clientAddr)) <0){
        printf("binding failed\n");        
    }
    listen(frameSocket,10);

}

void Client::sendMessage(){
    // Look for mouse and key events ans send these to server

}


void Client::bufferRequest(int socket){
    /// asking for frame buffers periodically
    while (true){
        FrameBufferUpdateRequest bufferReq;
        bufferReq.incremental = false;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}

void Client::setupFrameConnection(){
    sockaddr_in serAddr;
    int len = sizeof(serAddr);
    int frameBufferSocket = accept(frameSocket,(sockaddr*)&serverAddr,(socklen_t*)&len);
    // exchanging the server init messages

    ServerInit serverInit;
    int recvCount;
    while (recvCount <=0){
        recvCount = recv(clientSocket,&serverInit,sizeof(serverInit),0);
        if (recvCount == 0){
            printf("not received the server init message\n");
        }
    }
    screen_xres = serverInit.bufferWidth;
    screen_yres = serverInit.bufferHeight;
    bitsPerPixel = serverInit.pf.bitsPerPix;
    maxSize = serverInit.maxReqSize;
    isControl = serverInit.isControl;

    workers.push_back(std::thread(&Client::bufferRequest,this,frameBufferSocket));
    if (isControl) workers.push_back(std::thread(&Client::sendMessage,this));

}

void Client::initiateConnection(){
    if (connect(clientSocket,(sockaddr *)&serverAddr,sizeof(serverAddr)) <0 ){
        printf("Not able to connect\n");
    }
    setupFrameConnection();
}


void Client::handleGUIevents(){

    // depending on which button pressed on gui
    // call those functions in seperate thread;

}