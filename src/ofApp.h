#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
	
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
	
        vector<string> editStr;
        vector<string> playStr;
    
        // ~ ~ ~ edit ~ ~ ~
		ofTrueTypeFont editFont;
        int editFontSize;
        float editLetterSpacing;
        float editLineHeight;
        int editLeftMargin;
        int editTopMargin;
    
        ofColor editFontColor;
        ofColor editFontHighlightColor;
        ofColor editBgColor;

        int editCounter;

        // ~ ~ ~ play ~ ~ ~
        ofTrueTypeFont playFont;
        int playFontSize;
        float playLetterSpacing;
        float playLineHeight;
        int playLeftMargin;
        int playTopMargin;

        ofColor playFontColor;
        ofColor playBgColor;

        int playCounter;
    
        // ~ ~ ~ ~ ~ ~ ~
        int width;
        int height;
    
        ofFbo fbo;
        ofShader shader;
    
        bool editMode;
        
};

