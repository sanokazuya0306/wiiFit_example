#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context
//        ofSetupOpenGL(720,540,OF_WINDOW);
//        ofSetupOpenGL(480,320,OF_WINDOW);
//        ofSetupOpenGL(1280, 800,OF_WINDOW);
//    ofSetupOpenGL(1280,1024,OF_WINDOW);
//    ofSetupOpenGL(1920,1080,OF_WINDOW);
//    ofSetupOpenGL(1440,900,OF_WINDOW);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
