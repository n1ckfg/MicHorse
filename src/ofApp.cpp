#include "ofApp.h"

/*
TODO:
Scrolling
Swapping entries
Fix yellow last line
*/

//--------------------------------------------------------------
void ofApp :: setup() {
    ofm.setup(width, height);
    
    checkerboard.loadImage("textures/checkerboard.png");
    
    fbo.allocate(width, height, GL_RGBA);
    shaderName = "shaders/LED_GLES";
    shaderTexName = "tex0";
    shader.load(shaderName + ".frag", shaderName + ".vert");
    
    plane.set(width, height);   // dimensions for width and height in pixels
    plane.setPosition(width/2, height/2, 0); // position in x y z
    plane.setResolution(2, 2); // this resolution (as columns and rows) is enough
    saveKeystoneVertsOrig();
    keystoneStep = 10;
    keystoneIndex = 0;
    keystoneHandleColor = ofColor(255,0,0);
    keystoneHandleSize = 50;
    keystoneHandleStroke = 10;
    
    modeSelector = EDIT;
    
    editCounter = 0;
    playCounter = 0;
    swapCounter = -1;
    
	// old oF default is 96 - but this results in fonts looking larger than in other programs.
	ofTrueTypeFont :: setGlobalDpi(72);

    editFontSize = 30;
    editLineHeight = 34.0f;
    editLetterSpacing = 1.035;
	editFont.loadFont("fonts/verdana.ttf", editFontSize, true, true);
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
    playFont.loadFont("fonts/verdana.ttf", playFontSize, true, true);
    playFont.setLineHeight(playLineHeight);
    playFont.setLetterSpacing(playLetterSpacing);
    playLeftMargin = 100;
    playTopMargin = (height/2) + (playFontSize/2);
    
    playFontColor = ofColor(255,225,225);
    playBgColor = ofColor(0,127,0);

    editStr.push_back("");
    playStr.push_back("");
}

//--------------------------------------------------------------
void ofApp :: update() {
    //centerPlayText();
}

//--------------------------------------------------------------
void ofApp :: draw() {
    ofBackground(0);
    
    fbo.begin();
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
        ofSetColor(255); // why does this work?
    fbo.end();
    
    fbo.getTextureReference().bind();
    plane.draw();
    fbo.getTextureReference().unbind();
    
    if (modeSelector == KEYSTONE) {
        ofVec2f center = ofVec2f(plane.getMesh().getVertex(keystoneIndex).x + plane.getPosition().x, plane.getMesh().getVertex(keystoneIndex).y + plane.getPosition().y);
        ofPath circle;
        circle.setFillColor(ofColor(255,0,0));
        circle.arc(center, keystoneHandleSize + (keystoneHandleStroke/2), keystoneHandleSize + (keystoneHandleStroke/2), 0, 360);
        circle.close();
        circle.arc(center, keystoneHandleSize - (keystoneHandleStroke/2), keystoneHandleSize - (keystoneHandleStroke/2), 0, 360);
        circle.draw();
        
        //ofEllipse(plane.getMesh().getVertex(keystoneIndex).x + plane.getPosition().x, plane.getMesh().getVertex(keystoneIndex).y + plane.getPosition().y, 50, 50);
    }
}

//--------------------------------------------------------------
void ofApp :: keyPressed(int key) {
    if (key == OF_KEY_TAB) {
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
        if (ofm.KeyDelete(key)) {
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
        } else if (key == '/') { //KeyControl()) {
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
        } else if (key == OF_KEY_UP && editCounter > 0) {
            editCounter--;
        } else if (key == OF_KEY_DOWN || ofm.KeyReturn(key)) {
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
        } else if (!ofm.KeyIsArrow(key)) {
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
        } else if (key == 'z' || key == 'Z') {
            playCounter--;
            if (playCounter < 0) playCounter = editStr.size()-1;
         }
        if (editStr[playCounter].length() > 0) playStr[0] = editStr[playCounter];//ofSplitString(editStr[editCounter], "-");
        
    } else if (modeSelector == KEYSTONE) {
        if (key == '1') {
            keystoneIndex = 0;
        } else if (key == '2') {
            keystoneIndex = 1;
        } else if (key == '3') {
            keystoneIndex = 3;
        } else if (key == '4') {
            keystoneIndex = 2;
        } else if (key == '5') {
            loadKeystoneVertsOrig();
        } else if (ofm.KeyIsArrow(key)){
            keystoneVertex(keystoneIndex, key);
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
void ofApp :: keystoneVertex(int index, int key) {
    ofVec3f v = plane.getMesh().getVertex(index);
    
    if (key == OF_KEY_UP) {
        v.y -= keystoneStep;
    } else if (key == OF_KEY_DOWN) {
        v.y += keystoneStep;
    } else if (key == OF_KEY_LEFT) {
        v.x -= keystoneStep;
    } else if (key == OF_KEY_RIGHT) {
        v.x += keystoneStep;
    }
    
    plane.getMesh().setVertex(index, v);
}


void ofApp :: saveKeystoneVertsOrig() {
    for (int i=0; i<plane.getMesh().getVertices().size(); i++) {
        ofVec3f v = plane.getMesh().getVertex(i);
        keystoneVertsOrig.push_back(v);
    }
}

void ofApp :: loadKeystoneVertsOrig() {
    for (int i=0; i<keystoneVertsOrig.size(); i++) {
        plane.getMesh().setVertex(i, keystoneVertsOrig[i]);
    }
}

//--------------------------------------------------------------

