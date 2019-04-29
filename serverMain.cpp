#include "server.h"
#include <iostream>
#include <thread>

int main(){
    Server S(10);
    
    S.listenConnections();
    
}