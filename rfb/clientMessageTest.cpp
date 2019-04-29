#include "client.h"
#include <iostream>
#include "gui.h"

int main(){
    
    Client C;
    C.initiateConnection();

    while (true){

        // handle key events here 
        // call C.sendMessage(KeyEvent *, PointerEvent *);
    }

}