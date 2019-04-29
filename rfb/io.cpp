#include "io.h"


IO::IO(){
    dpy = XOpenDisplay(0);
    if (dpy ==NULL){
        printf("Cannot initialize the display\n");
        exit(2);
    }
    screen = XDefaultScreen(dpy);
    root_window = XRootWindow(dpy,screen);
    def_root_window = XDefaultRootWindow(dpy);
    display_height = DisplayHeight(dpy,screen);
    display_width = DisplayWidth(dpy,screen);

}


void IO::mouseClick(int button){
    XButtonEvent buttonEvent;
    buttonEvent.same_screen = True;
    buttonEvent.subwindow = None;
    buttonEvent.window = PointerWindow;

    buttonEvent.type = ButtonPress;
    buttonEvent.button = button;

    XQueryPointer(dpy,RootWindow(dpy,DefaultScreen(dpy)),&buttonEvent.root,&buttonEvent.window,&buttonEvent.x_root,&buttonEvent.y_root,&buttonEvent.x,&buttonEvent.y,&buttonEvent.state);
    XSendEvent(dpy,PointerWindow,True,0xfff,(XEvent*)&buttonEvent);
    XFlush(dpy);

    buttonEvent.type = ButtonRelease;
    buttonEvent.state = 0x100;
    XSendEvent(dpy,PointerWindow,True,0xfff,(XEvent*)&buttonEvent);


}


void IO::moveMouse(int x,int y){
    XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);
    XFlush(dpy);
}

XKeyEvent IO::createKeyEvent(Display *display, Window &win,
                           Window &winRoot, bool press,
                           int keycode, int modifiers)
{
   XKeyEvent event;

   event.display     = display;
   event.window      = win;
   event.root        = winRoot;
   event.subwindow   = None;
   event.x           = 1;
   event.y           = 1;
   event.x_root      = 1;
   event.y_root      = 1;
   event.same_screen = True;
   event.keycode     = XKeysymToKeycode(display, keycode);
   event.state       = modifiers;

   if(press)
      event.type = KeyPress;
   else
      event.type = KeyRelease;

   return event;
}

void IO::keyPress_release(int key){
       Window winFocus;
   int revert;
   XGetInputFocus(dpy, &winFocus, &revert);

   XKeyEvent event = createKeyEvent(dpy, winFocus, def_root_window, true, key, 0);
   XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);

   event = createKeyEvent(dpy, winFocus, def_root_window, false, key, 0);
   XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
}

