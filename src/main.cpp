#include "ofMain.h"
#include "ofApp.h"

//#ifdef TARGET_OPENGLES
    //#include "ofGLProgrammableRenderer.h"
//#endif

int main() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    #ifdef TARGET_OPENGLES
        ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    #endif
    
    ofSetupOpenGL(1280, 720, OF_FULLSCREEN);
    ofRunApp(new ofApp());
    
}
