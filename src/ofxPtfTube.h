#pragma once
#include "ofMain.h"

class ofxPtfTube {
public:
    struct SpinePoint {
        struct orientation {
            ofVec3f up;
            ofVec3f forward;
            ofVec3f side;
        } orientation;
        
        ofFloatColor color;
        ofVec3f loc;
    };
    
    ofxPtfTube();
    void update();
    void draw();

    void addSegment(ofVec3f segment);
    void addColor(ofColor color);
    void addRadius(float radius);
    
    void generate();
    void calculatePTF( vector<SpinePoint>& spine );
    void createVboFromSpine( ofVboMesh& vboMesh, const vector<SpinePoint>& spine, float width=1.0f, float thickness=1.0f );
    void drawSpineAxes( const vector<SpinePoint>& spine );
    void addTwoOneFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofColor col);
    void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d, ofColor col);
    
    ofVboMesh spineMeshes;

    ofPolyline      polyline;
    vector<SpinePoint> spines;

    vector<ofPoint> verts;
    vector<ofColor> colors;
    vector<float> radiuses;
};
