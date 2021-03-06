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
    
    ofxPtfTube(int num_segments = 32);
    void update();
    void draw();

    void addSegment(ofVec3f segment);
    void addColor(ofColor color);
    void addRadius(float radius);
    void clear();
    
    void generate();
    void calculatePTF( vector<SpinePoint>& spine );
    void createVboFromSpine( ofMesh& vboMesh, const vector<SpinePoint>& spine, float width=1.0f, float thickness=1.0f );
    void drawSpineAxes( const vector<SpinePoint>& spine );
    void addTwoOneFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofColor col);
    void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d, ofColor col);
    
    ofMesh spineMeshes;

    ofPolyline      polyline;
    vector<SpinePoint> spines;

    vector<ofPoint> segments;
    vector<ofColor> colors;
    vector<float> radiuses;
    
    int num_segments;
};
