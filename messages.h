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
    int bufferWidth;
    int bufferHeight;
    struct PixelFormat pf;
    char name[200];
};

struct SetPixelFormat{
    char messageType = '0';
    bool toSet;
    struct PixelFormat pf;
};

struct Encodings{

};

struct SetEncodings{
    char messageType = '2';
    bool toSet;
    struct Encodings encoding ;
};

struct FrameBufferUpdateRequest{
    char messageType = '3';
    bool incremental;
    int x_position;
    int y_position;
    int width;
    int height;
};

struct KeyEvent{
    char messageType = '4';
    bool downFlag;
    int key;
};

struct PointerEvent{
    char messageType  = '5';
    char buttonMask[8];
    int x_position;
    int y_position;

};

struct ClientCutText{
    char messageType = '6';
    int length;
    char text[200];
};




struct FrameBufferUpdate{
    char messageType = '0';
    int numRectangles;
    int x_position;
    int y_position;
    int width;
    int height;
    int ecodingType;
};

struct SetColorMapEntries{
    char messageType = '1';
    int firstColor;
    int numberOfColors;
    int red;
    int green;
    int blue;
};

struct Bell{
    char messageType = '2';

};

struct ServerCutText{
    char messageType = '3';
    int lenght;
    char text[200];
};



struct ServerMessage{
    FrameBufferUpdate buffUpdate;
    SetColorMapEntries colorMap;
    Bell   bell;
    ServerCutText cutText;
};


struct ClientMessage{
    SetPixelFormat pixelFromat;
    SetEncodings encodings;
    FrameBufferUpdateRequest request;
    KeyEvent key;
    PointerEvent pointer;
    ClientCutText cutText;
};

struct Encodings{
    
};
