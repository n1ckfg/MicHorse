#include "ofApp.h"

/*
TODO:
Scrolling
Swapping entries
Fix yellow last line
*/

//--------------------------------------------------------------
void ofApp :: setup() {
    width = ofGetWidth();
    height = ofGetHeight();
    
    checkerboard.loadImage("checkerboard.png");
    
    modeSelector = EDIT;
    keyboardType = RPI;
    
    editCounter = 0;
    playCounter = 0;
    swapCounter = -1;
    
	//old oF default is 96 - but this results in fonts looking larger than in other programs.
	ofTrueTypeFont :: setGlobalDpi(72);

    editFontSize = 30;
    editLineHeight = 34.0f;
    editLetterSpacing = 1.035;
	editFont.loadFont("verdana.ttf", editFontSize, true, true);
	editFont.setLineHeight(editLineHeight);
	editFont.setLetterSpacing(editLetterSpacing);
    editLeftMargin = 90;
    editTopMargin = 180;
    
    editFontColor = ofColor(225,225,225);
    editFontHighlightColor = ofColor(255,255,0);
    swapFontHighlightColor = ofColor(255,0,0);
    playFontHighlightColor = ofColor(0,255,255);
    editBgColor = ofColor(127,0,0);

    playFontSize = 60;
    playLineHeight = 68.0f;
    playLetterSpacing = 1.035;
    playFont.loadFont("verdana.ttf", playFontSize, true, true);
    playFont.setLineHeight(playLineHeight);
    playFont.setLetterSpacing(playLetterSpacing);
    playLeftMargin = 100;
    playTopMargin = (height/2) + (playFontSize/2);
    
    playFontColor = ofColor(255,225,225);
    playBgColor = ofColor(0,127,0);

    ofHideCursor();
    
    editStr.push_back("");
    playStr.push_back("");
}

//--------------------------------------------------------------
void ofApp :: update() {
    //centerPlayText();
}

//--------------------------------------------------------------
void ofApp :: draw() {
    ofClear(255,255,255, 0);
    
    if (modeSelector == EDIT || modeSelector == SWAP) {
        ofBackground(editBgColor);
        for (int i=0; i<editStr.size(); i++) {
            if (i == editCounter || i == swapCounter) {
                if (i == editCounter && (modeSelector != SWAP || i != swapCounter)) {
                    ofSetColor(editFontHighlightColor);
                } else if (modeSelector == SWAP && i == swapCounter && swapCounter != -1) {
                    ofSetColor(swapFontHighlightColor);
                }
            } else {
                if (i == playCounter) {
                    ofSetColor(playFontHighlightColor);
                } else {
                    ofSetColor(editFontColor);
                }
            }
            editFont.drawString(ofToString(i+1) + ". " + editStr[i], editLeftMargin, editTopMargin + (i * editLineHeight));
        }
    } else if (modeSelector == PLAY) {
        ofBackground(playBgColor);
        ofSetColor(playFontColor);
        playFont.drawString(playStr[0], playLeftMargin, playTopMargin);
    } else if (modeSelector == KEYSTONE) {
        ofSetColor(255);
        checkerboard.draw(0,0,width,height);
    }
    
}


//--------------------------------------------------------------
void ofApp :: keyPressed(int key) {

    if (key == KeyTab()) {
        if (modeSelector == EDIT) {
            modeSelector = PLAY;
        } else if (modeSelector == PLAY) {
            modeSelector = EDIT;
        } else if (modeSelector == SWAP) {
            swapCounter = -1;
            modeSelector = PLAY;
        }
    }
    
    if (modeSelector == EDIT || modeSelector == SWAP) {
        if (key == KeyDelete() || key == KeyBackspace()) {
            if (modeSelector == EDIT) {
                if (editStr[editCounter].length() > 0) {
                    editStr[editCounter] = editStr[editCounter].substr(0, editStr[editCounter].length()-1);
                } else if (editCounter != 0) {
                    //editStr.erase(editStr.begin() + editCounter);
                    //editCounter--;
                    //if (editCounter < 0) editCounter = 0;
                }
            } else if (modeSelector == SWAP) {
                if (swapCounter != -1) {
                    if (editStr.size() > 1) {
                        editStr.erase(editStr.begin() + swapCounter);
                        if (editStr.size() > 1) {
                            editCounter--;
                        } else {
                            editCounter = 0;
                        }
                
                    } else if (editStr.size() == 1) {
                        editStr[0] = "";
                    }
                }
                swapCounter = -1;
                modeSelector = EDIT;
                if (editCounter <0) editCounter = 0;
            }
        } else if (key == KeyControl()) {
            if (modeSelector == EDIT) {
                swapCounter = editCounter;
                modeSelector = SWAP;
            } else if (modeSelector == SWAP) {
                //if (editCounter != swapCounter) {
                if (swapCounter < 0) {
                    swapCounter = 0;
                } else if (swapCounter > editStr.size()-1) {
                    swapCounter = editStr.size()-1;
                }
                string swapLine = editStr[swapCounter];
                editStr[swapCounter] = editStr[editCounter];
                editStr[editCounter] = swapLine;
                //}
                swapCounter = -1;
                modeSelector = EDIT;
            }
        } else if (key == KeyUpArrow() && editCounter > 0) {
            editCounter--;
        } else if (key == KeyDownArrow() || key == KeyReturn()) {// && editCounter < editStr.size()-1) {
            //editCounter++;
            if (editStr[editCounter].length() > 0) {
                if (editCounter == editStr.size()-1) {
                    editStr.push_back("");
                    editCounter = editStr.size()-1;
                } else {
                    editCounter++;
                }
            } else if (editCounter < editStr.size()-1) {
                editCounter++;
            }
        } else if (key != KeyUpArrow() && key != KeyDownArrow() && key != KeyLeftArrow() && key != KeyRightArrow()) {
            //if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9') {
            if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5') {
                modeSelector = KEYSTONE;
            } else {            
                if (editCounter < 0) {
                    editCounter = 0;
                } else if (editCounter > editStr.size()-1) {
                    editCounter = editStr.size()-1;
                }
                editStr[editCounter].append(1, (char)key);
            }
        }
        
    } else if (modeSelector == PLAY) {
        if (key == ' ') {
            playCounter++;
            if (playCounter > editStr.size()-1 || (editStr[playCounter].length() < 1 && playCounter == editStr.size()-1)) playCounter = 0;
        } else if (key == 'z' || key == 'Z' || key == KeyBackspace() || key == KeyDelete()) {
            playCounter--;
            if (playCounter < 0) playCounter = editStr.size()-1;
         }
        if (editStr[playCounter].length() > 0) playStr[0] = editStr[playCounter];//ofSplitString(editStr[editCounter], "-");
        
    } else if (modeSelector == KEYSTONE) {
        if (key == '1') {
         
        } else if (key == '2') {
        
        } else if (key == '3') {
        
        } else if (key == '4') {

        } else if (key == '5') {
        
        } else {
            modeSelector = EDIT;
        }
    }
}

//--------------------------------------------------------------
void ofApp :: keyReleased(int key) { 
	
}

//--------------------------------------------------------------
void ofApp :: mouseMoved(int x, int y ) {
	
}

//--------------------------------------------------------------
void ofApp :: mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp :: mousePressed(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp :: mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp :: windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp :: gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp :: dragEvent(ofDragInfo dragInfo) { 

}

//--------------------------------------------------------------
void ofApp :: centerPlayText() {
    if (playStr.size() > 0 && playCounter <= playStr.size()-1) {
        playLeftMargin = (width/2) - ((playStr[playCounter].length() * playFontSize)/2);
    }
}
//--------------------------------------------------------------
int ofApp :: KeyTab() {
    if (keyboardType == OSX) {
        return OF_KEY_TAB;
    } else if (keyboardType == RPI) {
        return 9;
    }
}

int ofApp :: KeyDelete() {
    if (keyboardType == OSX) {
        return OF_KEY_DEL;
    } else if (keyboardType == RPI) {
        return 127;
    }
}

int ofApp :: KeyBackspace() {
    if (keyboardType == OSX) {
        return OF_KEY_BACKSPACE;
    } else if (keyboardType == RPI) {
        return 127;
    }
}

int ofApp :: KeyControl() {
    if (keyboardType == OSX) {
        return OF_KEY_CONTROL;
    } else if (keyboardType == RPI) {
        return 17;
    }
}

int ofApp :: KeyUpArrow() {
    if (keyboardType == OSX) {
        return OF_KEY_UP;
    } else if (keyboardType == RPI) {
        return 30;
    }
}

int ofApp :: KeyDownArrow() {
    if (keyboardType == OSX) {
        return OF_KEY_DOWN;
    } else if (keyboardType == RPI) {
        return 31;
    }
}

int ofApp :: KeyLeftArrow() {
    if (keyboardType == OSX) {
        return OF_KEY_LEFT;
    } else if (keyboardType == RPI) {
        return 28;
    }
}

int ofApp :: KeyRightArrow() {
    if (keyboardType == OSX) {
        return OF_KEY_RIGHT;
    } else if (keyboardType == RPI) {
        return 29;
    }
}

int ofApp :: KeyReturn() {
    if (keyboardType == OSX) {
        return OF_KEY_RETURN;
    } else if (keyboardType == RPI) {
        return 13;
    }
}

int ofApp :: KeyEnter() {
    if (keyboardType == OSX) {
        return 3;
    } else if (keyboardType == RPI) {
        return 3;
    }
}


