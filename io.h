#include<X11/Xlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/keysym.h>
class IO{
    private:
        Display * dpy;
        int screen;
        Window root_window;
        int display_height;
        int display_width;
    public:
        IO();
        void mouseClick(char *);
        void moveMouse(int ,int);
        void keyPress_release(int);
        XKeyEvent createKeyEvent(Display,Window,Window,bool,int,int);
};

