#pragma once

#include "ofMain.h"

class ofManager : public ofBaseApp {

    public:
        enum KeyboardType { OSX, RPI };
        KeyboardType keyboardType;
    
        void setup(int& width, int& height);
    
        bool KeyTab(int key);
        bool KeyDelete(int key);
        bool KeyControl(int key);
        bool KeyUpArrow(int key);
        bool KeyDownArrow(int key);
        bool KeyLeftArrow(int key);
        bool KeyRightArrow(int key);
        bool KeyReturn(int key);
    
        bool KeyIsArrow(int key);
        bool KeyIsNumber(int key);

};