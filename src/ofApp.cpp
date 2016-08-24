#include "ofApp.h"

int i;
int colorR[100];
int colorG[100];
int colorB[100];
int colorMaintain = 500;

float tunnelDiff = 200;
int tunnelNum = 50;
float forMove = 0;
float moveSpeed = 5;
float cameraDepth = -1000;
float lineWidth[100];
float lineZ[100];
float tunnelRotateParamX, tunnelRotateParamY, tunnelRotateParamZ;

Boolean cameraSwitch = false;
float camDist = 100;

int tunnelFlagX[2];
float rotateMaxX = 0.3;
int tunnelFlagY[2];
float rotateMaxY = 0.3;

//change speed
float tunnelSpinSpeedX = 0.015;
float tunnelSpinSpeedY = 0.015;

float ambient = 1;
float diffuse = 0;
float specular = 0;

//wii
float wiiFit[10];
float bottomLeft;
float bottomRight;
float topLeft;
float topRight;
float sum;
float virtualX;
float virtualY;
float sumSpeedAdd = 100;

float phase;

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetWindowPosition(1441,0);
//    ofSetFullscreen(true);
    
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
//    ofEnableDepthTest();
    ofSetFrameRate(60);
    
    
    //light
    light.enable();
    light.setPointLight();
    light.setPosition(ofGetWidth()/2, ofGetHeight()/2, cameraDepth);

    //color setting
    for(i=0; i<tunnelNum; i++) {
        colorR[i] = int(ofRandom(0,255));
        colorG[i] = int(ofRandom(0,colorMaintain-colorR[0]));
        if(colorMaintain - colorR[i] - colorG[i] > 255){
            colorB[i] = int(ofRandom(0,colorMaintain-colorR[i]-colorG[i]));
        } else {
            colorB[i] = colorMaintain - colorR[i] - colorG[i];
        }
    }
    
    cam.setDistance(100);
    
    //osc
    cout << "listening for oscmessages on port " << PORT << "\n";
    receiver.setup(PORT);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    phase += 0.01;
    
    //osc
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        for(i=0;i<7;i++){
            if(m.getAddress() == "/wii/1/balance/"+ofToString(i)){
                wiiFit[i] = m.getArgAsFloat(0);
            }
        }
    }
    
    //wii
    sum = ofMap(wiiFit[4], 0, 1.5, 0, 1);
    virtualX = ofMap(wiiFit[5], 0, 1, -tunnelSpinSpeedX, tunnelSpinSpeedX);
    virtualY = ofMap(wiiFit[6], 0, 1, -tunnelSpinSpeedY, tunnelSpinSpeedY);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
//    ofBackground(255, 255, 255);

    //light
    light.setAmbientColor(ofFloatColor(ambient,ambient,ambient,ambient));
    light.setDiffuseColor(ofFloatColor(diffuse,diffuse,diffuse));
    light.setSpecularColor(ofFloatColor(specular,specular,specular));

    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);

    ofTranslate(0, 0, cameraDepth);
    cam.setPosition(0, 0, 0);
    cam.begin();
    ofTranslate(0, 0, -cameraDepth);
    
    ofNoFill();
    
    for (i=0; i<tunnelNum; i++){
        ofSetColor(colorR[i], colorG[i], colorB[i]);
        
        lineZ[i] = -tunnelDiff * tunnelNum + i * tunnelDiff + forMove;
        lineWidth[i] = ofMap(abs(lineZ[i] - cameraDepth), 0, 500, 8, 1);
        
        ofSetLineWidth(lineWidth[i]);
        
        ofDrawBox(0, 0, lineZ[i], ofGetWidth()*0.2, ofGetHeight()*0.2, 1);
        if(i>0) {
            ofDrawLine(-ofGetWidth()*0.2, -ofGetHeight()*0.2, lineZ[i], -ofGetWidth()*0.2, -ofGetHeight()*0.2, lineZ[i-1]);
            ofDrawLine(ofGetWidth()*0.2, -ofGetHeight()*0.2, lineZ[i], ofGetWidth()*0.2, -ofGetHeight()*0.2, lineZ[i-1]);
            ofDrawLine(-ofGetWidth()*0.2, ofGetHeight()*0.2, lineZ[i], -ofGetWidth()*0.2, ofGetHeight()*0.2, lineZ[i-1]);
            ofDrawLine(ofGetWidth()*0.2, ofGetHeight()*0.2, lineZ[i], ofGetWidth()*0.2, ofGetHeight()*0.2, lineZ[i-1]);

            ofRotateX(tunnelRotateParamX);
            if(virtualX>0){
                if(tunnelRotateParamX >= rotateMaxX){
                    tunnelFlagX[0] = 0;
                } else {
                    tunnelFlagX[0] = 1;
                }
                
                if(tunnelFlagX[0] == 1){
                    tunnelRotateParamX += virtualX*virtualX;
                }
                
            } else {
                if(tunnelRotateParamX <= -rotateMaxX){
                    tunnelFlagX[1] = 0;
                } else {
                    tunnelFlagX[1] = 1;
                }
                if(tunnelFlagX[1] == 1){
                    tunnelRotateParamX -= virtualX*virtualX;
                }
            }
        
            ofRotateY(tunnelRotateParamY);
            if(virtualY>0){
                if(tunnelRotateParamY >= rotateMaxY){
                    tunnelFlagY[0] = 0;
                } else {
                    tunnelFlagY[0] = 1;
                }
                if(tunnelFlagY[0] == 1){
                    tunnelRotateParamY += virtualY*virtualY;
                }
                
            } else {
                if(tunnelRotateParamY <= -rotateMaxY){
                    tunnelFlagY[1] = 0;
                } else {
                    tunnelFlagY[1] = 1;
                }
                if(tunnelFlagY[1] == 1){
                    tunnelRotateParamY -= virtualY*virtualY;
                }
            }
        }
    }
    
    if(forMove < tunnelDiff) {
        forMove += moveSpeed + sum * sumSpeedAdd;
    } else {
        forMove = 0;
        for (i=0; i<tunnelNum; i++) {
            colorR[tunnelNum-i] = colorR[tunnelNum-i-1];
            colorG[tunnelNum-i] = colorG[tunnelNum-i-1];
            colorB[tunnelNum-i] = colorB[tunnelNum-i-1];
        }
        colorR[0] = int(ofRandom(0,255));
        colorG[0] = int(ofRandom(0,colorMaintain-colorR[0]));
        if(colorMaintain - colorR[0] - colorG[0] > 255){
            colorB[0] = int(ofRandom(0,colorMaintain-colorR[0]-colorG[0]));
        } else {
            colorB[0] = colorMaintain - colorR[0] - colorG[0];
        }
    }

    cam.end();
    
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2, 0);
    
    ofSetColor(255);
    ofDrawBitmapString("sum : "+ofToString(sum), 30,30);
    ofDrawBitmapString("virtualX : "+ofToString(virtualX), 30,60);
    ofDrawBitmapString("virtualY : "+ofToString(virtualY), 30,90);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
