#include<X11/Xlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/keysym.h>

#pragma once

class IO{
    private:
        Display * dpy;
        int screen;
        Window root_window;
        int display_height;
        int display_width;
        Window def_root_window;
        XKeyEvent createKeyEvent(Display*,Window&,Window&,bool,int,int);
    public:
        IO();
        void mouseClick(int);
        void moveMouse(int ,int);
        void keyPress_release(int);
        XKeyEvent createKeyEvent(Display,Window,Window,bool,int,int);
};

