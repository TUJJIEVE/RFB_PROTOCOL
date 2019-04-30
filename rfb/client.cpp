#include "client.h"
#define SI 100
// To Do : ask frame buffers in a patter
// check for key and mouse events

Client::Client(){
    shutDown = false;
    clientSocket = socket(AF_INET,SOCK_STREAM,0);
    frameSocket = socket(AF_INET,SOCK_STREAM,0);
    if (clientSocket == -1 || frameSocket == -1) printf("Not able to create socket\n");
    printf("socket created\n");
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5093);
    serverAddr.sin_addr.s_addr =htonl(INADDR_ANY);// inet_addr("192.168.1.2"); //htonl(INADDR_ANY);

}

void Client::setGUI(GUI * gui){
    guiWindow = gui;


}

void Client::sendMessage(KeyEvent key,PointerEvent pointer){
    // Look for mouse and key events ans send these to server
    CliMessage key_pointerEvent;
    printf("Sending messages\n");
    if (isControl){
        if ( key.downFlag == false){
            key_pointerEvent.key.downFlag = false;
            key_pointerEvent.pointer.button = pointer.button;
        
            key_pointerEvent.pointer.x_position = pointer.x_position;
            key_pointerEvent.pointer.y_position = pointer.y_position;
            key_pointerEvent.pointer.isMoved = pointer.isMoved;
            key_pointerEvent.pointer.isPressed = pointer.isPressed;
            int sendcount =0;
            sendcount = send(clientSocket,&key_pointerEvent,sizeof(key_pointerEvent),0);
            printf("pointer event sent %d %d",sendcount,sizeof(key_pointerEvent));

        }

        else if  (key.downFlag == true){
            key_pointerEvent.key.key = key.key;
            key_pointerEvent.key.downFlag = true;
            int sendcount =0;
            sendcount = send(clientSocket,&key_pointerEvent,sizeof(key_pointerEvent),0);
            printf("key event sent %d %d",sendcount,sizeof(key_pointerEvent));


        }



    }
    else{
        printf("no control rights so cannot control\n");
    }


}

void Client::prepareRectangles(FrameBufferUpdateRequest * buff,int x_start,int y_start){
    //for (int i=0;i<buff->numRectangles;i++){
        //for (int x = x_start,i = 0,y = y_start; x<screen_xres && y <screen_yres && i<buff->numRectangles;x+=SI,i++){
            buff->rectangleRequests[0].x_position = x_start;
            buff->rectangleRequests[0].y_position = y_start;
            buff->rectangleRequests[0].width = x_start+SI> screen_xres ? screen_xres - x_start : SI;
            buff->rectangleRequests[0].height = y_start+SI > screen_yres ? screen_yres - y_start:SI;
        //}
    //}
}

void Client::bufferRequest(int socket,int x_start,int y_start){
    /// asking for frame buffers periodically
        //printf("buffer request\n");
        for (int x=x_start,y=y_start;x<screen_xres ; x+=SI){
            FrameBufferUpdateRequest bufferReq;
            bufferReq.incremental = false;
            bufferReq.numRectangles = 1;//ceil((double)screen_xres /(double)SI );
            bufferReq.isShuttingDown = false;
           //if (x>screen_yres) break;
            prepareRectangles(&bufferReq,x,y);
            int sendcount = send(socket,&bufferReq,sizeof(bufferReq),0);
            printf("sendcount %d %d\n",sendcount,sizeof(bufferReq));

            FrameBufferUpdate response;
            int totalCount = 0;
            int recvCount = 0;
            int tries = 0;
            printf("%d\n",sizeof(response));
            while (totalCount < sizeof(response)){
                printf("recv\n");
                recvCount = recv(socket,((char*)&(response))+(4*totalCount),sizeof(response) - totalCount,0);
                if (recvCount >0)totalCount += recvCount;
                if (recvCount == -1) tries++;
                else if (recvCount ==0) break;
                if (tries >5) break;
                printf("Recv %d total %d\n",recvCount,totalCount);
            }

            if (tries >5) continue;

            {
                std::unique_lock<std::mutex> l(m);
                frameQs.push(response);
            }
           // int recvCount = recv(socket,&response,sizeof(response),0);
            printf("The recv %d %d\n",totalCount,sizeof(response));
            printf("The number %d\n",response.numRectangles);

            //return;
        }
        return;
    

}

void Client::askForFrames(){
        for (int i=0;i< ceil(screen_yres/(double)SI);i++ ) bufferRequest(frameBufferSocket,0,i*100);// workers.push_back(std::thread(&Client::bufferRequest,this,frameBufferSocket,0,i*100));

}


void Client::setupFrameConnection(){

    ServerInit serverInit;
    int recvCount = 0;
    while (recvCount <=0){
        recvCount = recv(clientSocket,&serverInit,sizeof(serverInit),0);
        if (recvCount == 0){
            printf("not received the server init message\n");
        }
    }
    printf("recv count %d\n",recvCount);
    screen_xres = serverInit.bufferWidth;
    screen_yres = serverInit.bufferHeight;
    pixie.bitsPerPix = serverInit.pf.bitsPerPix;
    pixie.blue_shift = serverInit.pf.blue_shift;
    pixie.red_shift = serverInit.pf.red_shift;
    pixie.green_shift = serverInit.pf.green_shift;
    pixie.redMax = serverInit.pf.redMax;
    pixie.blueMax = serverInit.pf.blueMax;
    pixie.greenMax = serverInit.pf.greenMax;
    maxSize = serverInit.maxReqSize;
    isControl = serverInit.isControl;

    if (isControl){
        clientAddr.sin_family = AF_INET;
        clientAddr.sin_port = htons(CLIENT_PORT);
        printf("client port%d\n",clientAddr.sin_port);
        clientAddr.sin_addr.s_addr  =htonl(INADDR_ANY);// inet_addr("192.168.1.1"); //htonl(INADDR_ANY);
        
        if (bind(frameSocket,(sockaddr*)&clientAddr,sizeof(clientAddr)) <0){
            printf("binding failed\n");        
            exit(EXIT_FAILURE);
        }
        printf("binding sucessfull\n");
        listen(frameSocket,10);
        printf("setting frame connection\n");
        sockaddr_in serAddr;
        int len = sizeof(serAddr);
        printf("listening from server\n");
        frameBufferSocket = accept(frameSocket,(sockaddr*)&serverAddr,(socklen_t*)&len);
        // exchanging the server init messages
        printf("server connected for frame sending\n");



    }else{
        frameBufferSocket = clientSocket; 
    }
    if (isControl) printf("Got the control rights\n");

}

void Client::shutitDown(){
    printf("shutting down\n");
    shutDown = true;
    CliMessage shutMessage;
    shutMessage.isShuttingDown = true;
    send(clientSocket,&shutMessage,sizeof(shutMessage),0);
    FrameBufferUpdateRequest shutFrameLine;
    shutFrameLine.isShuttingDown = true;
    send(frameBufferSocket,&shutFrameLine,sizeof(shutFrameLine),0);
    close(clientSocket);
    close(frameBufferSocket);
}

void Client::initiateConnection(){
    printf("initiating connections\n");

    if (connect(clientSocket,(sockaddr *)&serverAddr,sizeof(serverAddr)) <0 ){
        printf("Not able to connect\n");
        exit(EXIT_FAILURE);
    }

    printf("connection to server established\n");
    setupFrameConnection();

}

