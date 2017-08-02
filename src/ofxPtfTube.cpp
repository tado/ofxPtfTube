#include "ofxPtfTube.h"

ofxPtfTube::ofxPtfTube(){
    segments.clear();
    radiuses.clear();
    colors.clear();
    spines.clear();
}


void ofxPtfTube::update(){
    
}

void ofxPtfTube::draw(){
    //drawSpineAxes(spines); // DEBUG
    vector<ofVboMesh>::iterator it;
    spineMeshes.drawFaces();
}

void ofxPtfTube::clear(){
    segments.clear();
    radiuses.clear();
    colors.clear();
    spines.clear();
}

void ofxPtfTube::generate(){
    ofVec3f lastVert;
    int index = 0;
    for ( int i=0; i<segments.size(); i++) {
        if ( lastVert.squareDistance( segments[i] ) > 0.1f ) {
            spines.push_back( SpinePoint() );
            spines[index].loc = segments[i];
            lastVert = segments[i];
            index++;
        }
    }
    calculatePTF(spines);
    createVboFromSpine(spineMeshes, spines, 4.0f, 1.0f);
}

void ofxPtfTube::addSegment(ofVec3f segment){
    segments.push_back(segment);
}
void ofxPtfTube::addColor(ofColor color){
    colors.push_back(color);
}
void ofxPtfTube::addRadius(float radius){
    radiuses.push_back(radius);
}

void ofxPtfTube::createVboFromSpine( ofVboMesh& vboMesh, const vector<SpinePoint>& spine, float width, float thickness ) {
    vboMesh.clear();
    int num_segments = 128;
    vector<ofVec3f> vertex;
    
    for ( int a=0; a<spine.size(); a++ ) {
        //float r = sin(a * 0.01) * 5.0 + 10.0;
        
        float r;
        if (radiuses.size() < segments.size()) {
            r = 10.0;
        } else {
            r = radiuses[a];
        }
        
        ofVec3f side = spine[a].orientation.up;
        float angle_to_rotate = TWO_PI/(float)num_segments;
        
        ofVec3f axis = spine[a].orientation.forward;
        
        for( int i = 0; i < num_segments; i++ ) {
            side.rotateRad( angle_to_rotate, axis );
            vertex.push_back( spine[a].loc + side * r);
        }
    }
    
    vector<int> indices;
    
    for ( int i=1; i<spine.size()-2; i++ ) {
        int index = i*num_segments;
        
        ofColor col;
        if (colors.size() < segments.size()) {
            col = ofColor(127);
        } else {
            col = colors[i];
        }
        
        for ( int j=0; j<num_segments-1; j++ ) {
            ofVec3f a = vertex[index + j];
            ofVec3f b = vertex[index + j + 1];
            ofVec3f c = vertex[index + j + num_segments + 1];
            ofVec3f d = vertex[index + j + num_segments];
            addFace( vboMesh, a, b, c, d , col);
        }
        
        // close end to beginning
        ofVec3f a = vertex[index + num_segments-1];
        ofVec3f b = vertex[index + 0];
        ofVec3f c = vertex[index + num_segments];
        ofVec3f d = vertex[index + num_segments + num_segments-1];
        
        addFace( vboMesh, a, b, c, d, col);
    }
}

void ofxPtfTube::drawSpineAxes( const vector<SpinePoint>& spine ) {
    for ( int i=0; i<spine.size()-1; i++ ) {
        
        // draw axis
        glBegin(GL_LINES);
        
        // rotated up
        glColor3f(0,1,0);
        glVertex3fv( spine[i].loc.getPtr() );
        glVertex3fv( (spine[i].loc + spine[i].orientation.up*5.0f).getPtr() );
        
        glColor3f(1,0,1);
        glVertex3fv( spine[i].loc.getPtr() );
        glVertex3fv( (spine[i].loc + spine[i].orientation.forward*5.0f).getPtr() );
        
        // corrected right
        glColor3f(1,0,0);
        glVertex3fv( spine[i].loc.getPtr() );
        glVertex3fv( (spine[i].loc + spine[i].orientation.side*5.0f).getPtr() );
        
        // connect points
        glColor3f(0.6,0.6,0.6);
        glVertex3fv( spine[i].loc.getPtr() );
        glVertex3fv( (spine[i].loc + spine[i+1].loc-spine[i].loc).getPtr() );
        
        glEnd();
        
        int num_segments = 10;
        
        glBegin(GL_LINE_LOOP);
        glColor3f(1.0, 0.0, 1.0);
        glVertex3fv( ( spine[i].loc + spine[i].orientation.side * 5.0f + spine[i].orientation.up * 5.0f ).getPtr() );
        
        glColor3f(1.0,0.0,0.0);
        glVertex3fv( ( spine[i].loc + spine[i].orientation.side * 5.0f - spine[i].orientation.up * 5.0f ).getPtr() );
        
        glColor3f(0.0, 1.0, 0.0);
        glVertex3fv( ( spine[i].loc - spine[i].orientation.side * 5.0f - spine[i].orientation.up * 5.0f ).getPtr() );
        
        glColor3f(0.0, 0.0, 1.0);
        glVertex3fv( ( spine[i].loc - spine[i].orientation.side * 5.0f + spine[i].orientation.up * 5.0f ).getPtr() );
        glEnd();
    }
}

void ofxPtfTube::calculatePTF( vector<SpinePoint>& spine ) {
    
    ofVec3f dirNormal;
    ofVec3f prevSide, prevForward, prevUp;
    
    // Initial point in spine
    dirNormal = (spine[1].loc - spine[0].loc).getNormalized();
    
    ofVec3f temp_up = ofVec3f(0.0f, 1.0f, 0.0f);
    
    ofVec3f new_forward = dirNormal;
    ofVec3f new_side = new_forward.getCrossed(temp_up).getNormalized();
    ofVec3f new_up = new_forward.getCrossed(new_side).getNormalized();
    
    spine[0].orientation.up = new_up;
    spine[0].orientation.forward = new_forward;
    spine[0].orientation.side = new_side;
    
    prevSide = new_side;
    prevForward = new_forward;
    prevUp = new_up;
    
    spine[0].color = ofFloatColor( 1.0f, 0.0f, 0.0f, 1.0f );
    
    for ( int i=1; i<spine.size()-1; ++i ) {
        ofVec3f T1(spine[i].loc - spine[i-1].loc);
        ofVec3f T2(spine[i+1].loc - spine[i].loc);
        T1.normalize();
        T2.normalize();
        
        ofVec3f A = T1.getCrossed(T2);
        
        if ( A.length() <= 0.001f ) { // is angle too small? cross product result of zero means lines are parallel
            new_up = prevUp;
            new_forward = prevForward;
            new_side = prevSide;
        } else {
            float alpha = T1.angleRad(T2);
            
            new_forward.rotateRad( alpha, A );
            new_side.rotateRad( alpha, A );
            new_up.rotateRad( alpha, A );
        }
        
        spine[i].orientation.up = new_up;
        spine[i].orientation.forward = new_forward;
        spine[i].orientation.side = new_side;
        
        prevSide = new_side;
        prevForward = new_forward;
        prevUp = new_up;
    }
}

void ofxPtfTube::addTwoOneFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofColor col) {
    mesh.addVertex(a);
    mesh.addVertex(b);
    mesh.addVertex(c);
    
    ofVec3f p0 = (a-b).getNormalized();
    ofVec3f p1 = (b-c).getNormalized();
    
    ofVec3f n = p0.getCrossed(p1).getNormalized();
    mesh.addNormal(n);
    mesh.addNormal(n);
    mesh.addNormal(n);
    
    mesh.addColor(col);
    mesh.addColor(col);
    mesh.addColor(col);
}

void ofxPtfTube::addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d, ofColor col) {
    addTwoOneFace(mesh, a, b, c, col);
    addTwoOneFace(mesh, a, c, d, col);
}
