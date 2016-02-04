#include "ofApp.h"

/*
TODO:
Scrolling
Swapping entries
Fix yellow last line
*/

//--------------------------------------------------------------
void ofApp :: setup() {
   
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetLogLevel("ofThread", OF_LOG_ERROR);
    consoleListener.setup(this);

    width = ofGetWidth();
    height = ofGetHeight();
    
    modeSelector = EDIT;
    
    editCounter = 0;
    playCounter = 0;
    swapCounter = -1;
    
    fbo.allocate(width, height, GL_RGBA);
    
    #ifdef TARGET_OPENGLES
        shader.load("shaders_gles/noise.vert","shaders_gles/noise.frag");
    #else
        if(ofGetGLProgrammableRenderer()){
            shader.load("shaders_gl3/noise.vert", "shaders_gl3/noise.frag");
        }else{
            shader.load("shaders/noise.vert", "shaders/noise.frag");
        }
    #endif
    doShader = false;
    
	//old oF default is 96 - but this results in fonts looking larger than in other programs.
	ofTrueTypeFont :: setGlobalDpi(72);

    editFontSize = 30;
    editLineHeight = 34.0f;
    editLetterSpacing = 1.035;
	editFont.loadFont("verdana.ttf", editFontSize, true, true);
	editFont.setLineHeight(editLineHeight);
	editFont.setLetterSpacing(editLetterSpacing);
    editLeftMargin = 90;
    editTopMargin = 230;
    
    editFontColor = ofColor(225,225,225);
    editFontHighlightColor = ofColor(255,255,0);
    swapFontHighlightColor = ofColor(255,0,0);
    editBgColor = ofColor(127,0,0);

    playFontSize = 60;
    playLineHeight = 68.0f;
    playLetterSpacing = 1.035;
    playFont.loadFont("verdana.ttf", playFontSize, true, true);
    playFont.setLineHeight(playLineHeight);
    playFont.setLetterSpacing(playLetterSpacing);
    playLeftMargin = 100;
    playTopMargin = 230;
    
    playFontColor = ofColor(255,225,225);
    playBgColor = ofColor(0,127,0);

    ofHideCursor();
    
    editStr.push_back("");
    playStr.push_back("");
}

//--------------------------------------------------------------
void ofApp :: update() {
    
}

//--------------------------------------------------------------
void ofApp :: draw() {
    //fbo.begin();
    //ofClear(255,255,255, 0);
    
    //ofBackground(0);
    
    if (doShader) {
        shader.begin();
        shader.setUniformTexture("tex0", fbo.getTextureReference(), fbo.getTextureReference().texData.textureID);
        shader.setUniform1f("time", ofGetElapsedTimef());
        shader.setUniform2f("resolution", width, height);
    }

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
                ofSetColor(editFontColor);
            }
            editFont.drawString(ofToString(i+1) + ". " + editStr[i], editLeftMargin, editTopMargin + (i * editLineHeight));
        }
    } else if (modeSelector == PLAY) {
        ofBackground(playBgColor);
        ofSetColor(playFontColor);
        playFont.drawString(playStr[0], playLeftMargin, playTopMargin);
    }
    
    if (doShader) {
        shader.end();
    }
    
    //fbo.end();

    //fbo.draw(0,0);
}


//--------------------------------------------------------------
void ofApp :: keyPressed(int key) {
    ofLog(OF_LOG_VERBOSE, "%c keyPressed", key);

    if (key == OF_KEY_TAB) {
        if (modeSelector == EDIT) {
            modeSelector = PLAY;
        } else if (modeSelector == PLAY) {
            modeSelector = EDIT;
        }
    }
    
    if (modeSelector == EDIT || modeSelector == SWAP) {
        if (key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) {
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
                    editStr.erase(editStr.begin() + swapCounter);
                }
                swapCounter = -1;
                modeSelector = EDIT;
            }
        } else if (key == OF_KEY_RETURN ) {
            /*
            if (editStr[editCounter].length() > 0) {
                if (editCounter == editStr.size()-1) {
                    editStr.push_back("");
                    editCounter = editStr.size()-1;
                } else {
                    editCounter++;
                }
            }
            */
            if (modeSelector == EDIT) {
                swapCounter = editCounter;
                modeSelector = SWAP;
            } else if (modeSelector == SWAP) {
                string swapLine = editStr[swapCounter];
                editStr[swapCounter] = editStr[editCounter];
                editStr[editCounter] = swapLine;
                swapCounter = -1;
                modeSelector = EDIT;
            }
        } else if (key == OF_KEY_UP && editCounter > 0) {
            editCounter--;
        } else if (key == OF_KEY_DOWN) {// && editCounter < editStr.size()-1) {
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
        } else if (key != OF_KEY_UP && key != OF_KEY_DOWN && key != OF_KEY_LEFT && key != OF_KEY_RIGHT) {
            //if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9') {
                //modeSelector = NUM_START;
            //} else {
                editStr[editCounter].append(1, (char)key);
            //}
        }
        
    } else if (modeSelector == PLAY) {
        if (key == ' ') {
            playCounter++;
            if (playCounter > editStr.size()-1 || (editStr[playCounter].length() < 1 && playCounter == editStr.size()-1)) playCounter = 0;
        } else if (key == 'z' || key == 'Z' || key == OF_KEY_BACKSPACE || key == OF_KEY_DEL) {
            playCounter--;
            if (playCounter < 0) playCounter = editStr.size()-1;
         }
        if (editStr[playCounter].length() > 0) playStr[0] = editStr[playCounter];//ofSplitString(editStr[editCounter], "-");
        
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
void ofApp :: onCharacterReceived(KeyListenerEventData& e)
{
    keyPressed((int)e.character);
}
