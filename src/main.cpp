#include "ofMain.h"
#include "ofApp.h"

#ifdef TARGET_RASPBERRY_PI
    #include "ofGLProgrammableRenderer.h"
#endif

int main() {
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    #ifdef TARGET_RASPBERRY_PI
        ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
    #endif
    
    ofSetupOpenGL(1280, 720, OF_FULLSCREEN);
    ofRunApp(new ofApp());
    
}
