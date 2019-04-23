/* Message structures will be defined */



struct clientInit{
    bool sharedFlag;
};



struct PixelFormat{
    int bitsPerPix;
    int depth;
    bool big_endian_flag;
    bool true_color_flag;
    int redMax;
    int greenMax;
    int blueMax;
    int red_shift;
    int blue_shift;
    int green_shift;

};


struct ServerInit{

    bool isControl;
    int bufferWidth;
    int bufferHeight;
    struct PixelFormat pf;
    char name[200];

};

struct SetPixelFormat{
    //char messageType = '0';
    bool toSet;
    struct PixelFormat pf;
};

struct Encodings{

};

struct SetEncodings{
    //char messageType = '2';
    bool toSet;
    struct Encodings encoding ;
};

struct Rectangle{
    
    int x_position;
    int y_position;
    int width;
    int height;
    int encodingType;
    Rectangle();
    Rectangle(int x_pos,int y_pos,int w,int h){
        x_position = x_pos;
        y_position = y_pos;
        width = w;
        height = h;
    }
    bool operator==(const Rectangle&rhs){
        return (this->x_position == rhs.x_position && this->y_position == rhs.y_position
                && this->height == rhs.height && this->width == rhs.width);
    }
};

struct FrameBufferUpdateRequest{
    //char messageType = '3';
    bool incremental;
    int numRectangles;
    Rectangle rectangleRequests[50];
};

struct KeyEvent{
    char messageType = '4';

    bool downFlag;
    int key;
};


struct PointerEvent{
    //char messageType  = '5';
    char buttonMask[8];
    int x_position;
    int y_position;
    bool isMoved = false;
    bool isPressed = false;
};

struct ClientCutText{
    //char messageType = '6';
    int length;
    //char text[200];
};
// What size to keep there could be error while sendig if pointer is used
struct RectangleInfo{
    char information[400];
};


struct FrameBufferUpdate{
    //char messageType = '0';
    int numRectangles;
    RectangleInfo rectangleResponse[50];
    FrameBufferUpdate(int num){
        numRectangles = num;
    }    
};

struct SetColorMapEntries{
    //char messageType = '1';
    int firstColor;
    int numberOfColors;
    int red;
    int green;
    int blue;
};

struct Bell{
    //char messageType = '2';

};

struct ServerCutText{
    //char messageType = '3';
    int lenght;
    //char text[200];
};



struct ServerMessage{
    FrameBufferUpdate buffUpdate;
    //SetColorMapEntries colorMap;
    Bell   bell;
    //ServerCutText cutText;
    ServerMessage(int num):buffUpdate(num){}
};


struct ClientMessage{
    SetPixelFormat pixelFromat;
    SetEncodings encodings;
    FrameBufferUpdateRequest request;
    KeyEvent key;
    PointerEvent pointer;
    ClientCutText cutText;
    bool isShuttingDown = false;
};

struct Encodings{

};
