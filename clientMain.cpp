#include "client.h"
#include <iostream>
#include "gui.h"

int main(){
    GUI gui;
    Client C(&gui);

    while (true){
        C.handleGUIevents();
    }
    

}