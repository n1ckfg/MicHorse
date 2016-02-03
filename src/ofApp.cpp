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
    
    editMode = true;
    editCounter = 0;
    playCounter = 0;
    
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
    fbo.begin();
    shader.begin();
    shader.setUniformTexture("tex0", fbo.getTextureReference(), fbo.getTextureReference().texData.textureID);
    shader.setUniform1f("time", ofGetElapsedTimef());
    shader.setUniform2f("resolution", width, height);

    ofClear(255,255,255, 255);

    if (editMode) {
        ofBackground(editBgColor);
        for (int i=0; i<editStr.size(); i++) {
            if (i == editCounter) {
                ofSetColor(editFontHighlightColor);
            } else {
                ofSetColor(editFontColor);
            }
            editFont.drawString(ofToString(i+1) + ". " + editStr[i], editLeftMargin, editTopMargin + (i * editLineHeight));
        }
    } else {
        ofBackground(playBgColor);
        ofSetColor(playFontColor);
        playFont.drawString(playStr[0], playLeftMargin, playTopMargin);
    }
    shader.end();
    fbo.end();

    fbo.draw(0,0);
}


//--------------------------------------------------------------
void ofApp :: keyPressed(int key) {
    ofLog(OF_LOG_VERBOSE, "%c keyPressed", key);

    if (key == OF_KEY_TAB) {
        editMode = !editMode;
    }
    
    if (editMode) {
        
        if (key == OF_KEY_DEL || key == OF_KEY_BACKSPACE) {
            if (editStr[editCounter].length() > 0) {
                editStr[editCounter] = editStr[editCounter].substr(0, editStr[editCounter].length()-1);
            } else if (editCounter != 0) {
                editStr.erase(editStr.begin() + editCounter);
                editCounter--;
                if (editCounter < 0) editCounter = 0;
            }
        } else if (key == OF_KEY_RETURN ) {
            if (editStr[editCounter].length() > 0) {
                if (editCounter == editStr.size()-1) {
                    editStr.push_back("");
                    editCounter = editStr.size()-1;
                } else {
                    editCounter++;
                }
            }
            // TODO sorting here
        } else if (key == OF_KEY_UP && editCounter > 0) {
            editCounter--;
        } else if (key == OF_KEY_DOWN && editCounter < editStr.size()-1) {
            editCounter++;
        } else if (key != OF_KEY_UP && key != OF_KEY_DOWN && key != OF_KEY_LEFT && key != OF_KEY_RIGHT){
            editStr[editCounter].append(1, (char)key);
        }
        
    } else {
        if (key == ' ') {
            playCounter++;
            if (playCounter > editStr.size()-1 || (editStr[playCounter].length() < 1 && playCounter == editStr.size()-1)) playCounter = 0;
        } else if (key == 'z' || key == 'Z') {
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
