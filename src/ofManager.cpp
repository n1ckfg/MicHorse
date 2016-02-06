#include "ofManager.h"

void ofManager :: setup(int& width, int& height) {
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofSetVerticalSync(true);
    ofDisableArbTex();
    //ofEnableDepthTest();
    //ofEnableSmoothing();
    //ofEnableNormalizedTexCoords();
    //ofEnableLighting();
    //ofEnableAntiAliasing();
    
    ofHideCursor();
    
    keyboardType = OSX;
    
    width = ofGetWidth();
    height = ofGetHeight();
}

//--------------------------------------------------------------

bool ofManager :: KeyTab(int key) {
    if (keyboardType == OSX && key == OF_KEY_TAB) {
        return true;
    } else if (keyboardType == RPI && key == 15) {
        return true;
    } else {
        return false;
    }
}

bool ofManager :: KeyDelete(int key) {
    if (keyboardType == OSX) {
        if (key == OF_KEY_DEL) {
            return true;
        } else if (key == OF_KEY_BACKSPACE) {
            return true;
        }
    } else if (keyboardType == RPI && (key == 111 || key == 14)) {
        return true;
    } else {
        return false;
    }
}

bool ofManager :: KeyControl(int key) {
    if (keyboardType == OSX) {
        if (key == OF_KEY_CONTROL) {
            return true;
        } else if (key == OF_KEY_RIGHT_CONTROL) {
            return true;
        } else if (key == OF_KEY_LEFT_CONTROL) {
            return true;
        }
    } else if (keyboardType == RPI && (key == 29 || key == 97)) {
        return true;
    } else {
        return false;
    }
}

bool ofManager :: KeyUpArrow(int key) {
    if (keyboardType == OSX && key == OF_KEY_UP) {
        return true;
    } else if (keyboardType == RPI && key == 103) {
        return true;
    } else {
        return false;
    }
}

bool ofManager :: KeyDownArrow(int key) {
    if (keyboardType == OSX && key == OF_KEY_DOWN ) {
        return true;
    } else if (keyboardType == RPI && key == 108) {
        return true;
    } else {
        return false;
    }
}

bool ofManager :: KeyLeftArrow(int key) {
    if (keyboardType == OSX && key == OF_KEY_LEFT) {
        return true;
    } else if (keyboardType == RPI && key == 105) {
        return true;
    } else {
        return false;
    }
}

bool ofManager :: KeyRightArrow(int key) {
    if (keyboardType == OSX && key == OF_KEY_RIGHT) {
        return true;
    } else if (keyboardType == RPI && key == 106) {
        return true;
    } else {
        return false;
    }
}

bool ofManager :: KeyReturn(int key) {
    if (keyboardType == OSX && key == OF_KEY_RETURN) {
        return true;
    } else if (keyboardType == RPI && (key == 28 || key == 96)) {
        return true;
    } else {
        return false;
    }
}

bool ofManager :: KeyIsArrow(int key) {
    if (KeyUpArrow(key) || KeyDownArrow(key) || KeyLeftArrow(key) || KeyRightArrow(key)) {
        return true;
    } else {
        return false;
    }
}

bool ofManager :: KeyIsNumber(int key) {
    if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9') {
        return true;
    } else {
        return false;
    }
}