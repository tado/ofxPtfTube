#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    light.setup();
    light.setPosition(-400, -400, 1000);
    ofEnableLighting();
    light.enable();
    
    tube = new ofxPtfTube();
    
    //generate curves
    float speed = 0.01;
    for (int i = 0; i < 4000; i++) {
        ofVec3f position;
        float r = sin(i * speed * 0.21) * 200.0;
        position.z = sin(i * speed) * r;
        position.x  = cos(i * speed * 1.2) * r;
        position.y  = sin(i * speed * 1.7) * r;
        float hue = abs(sin(i * speed) * 255);
        ofColor col;
        col.setHsb(hue, 127, 255);
        tube->addSegment(position);
        tube->addRadius(sin(i * speed * 3.0) * 12.0 + 26.0);
        tube->addColor(col);
    }
    tube->generate();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(127), ofColor(91));
    cam.begin();
    ofEnableDepthTest();
    tube->draw();
    ofDisableDepthTest();
    cam.end();
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
