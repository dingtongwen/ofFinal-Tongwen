#include "ofApp.h"



const int N = 1500;
float spectrum[N];

float cen1[N];
float cen2[N];
float cen3[N];

float ver1[N*3];
float ver2[N*3];
float ver3[N*3];

float Rad = 2000;
float Vel = 0.1;
int bandRad = 2;
int bandVel = 100;



float Rads;  //The sphere's radius
float rad;  //Maximal triangle's "radius"

const int n = 300;

float tx[n],ty[n];
ofPoint p[n];

float time0 = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    nFrames = 0;
    frameW  = 320;
    frameH  = 240;
    
    vid.initGrabber(frameW,frameH);
    
    gifEncoder.setup(frameW, frameH, .25, 256);
    ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);


    
    sound.loadSound("BurningBright.mp3");
    sound.setLoop(true);
    sound.play();
    
    for(int i = 0; i<N; i++){
        spectrum[i] = 0.0f;
        
    }
    
    for(int j = 0; j<n; j++){
        tx[j] = ofRandom(0,1000);
        ty[j] = ofRandom(0,1000);
    }
    
    
    
    
    nTri = 1500;  //The number of the triangles
    nVert= nTri * 3;  //The number of the vertices
    
    
    for (int i=0; i<nTri; i++){
        cen1[i] = ofRandom( -1,1 );
        cen2[i] = ofRandom( -1,1 );
        cen3[i] = ofRandom( -1,1 );
        
        for (int j=0; j<3; j++) {
        
        ver1[i*3 + j]=ofRandom( -25,25 );
            
        ver2[i*3 + j]=ofRandom( -25,25 );
            
        ver3[i*3 + j]=ofRandom( -25,25 );
            
            
        }
    }
    
    
    //Fill the array of triangles' colors
    colors.resize( nTri );
    for (int i=0; i<nTri; i++) {
        
        colors[i] = ofColor( ofRandom( 0, 255 ), 0, 0 );  //Take a random color from black to red
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    vid.update();
    
    ofSoundUpdate();
    
    float *val = ofSoundGetSpectrum(N);
    
    for(int i =0; i<N; i++){
        spectrum[i] *= 0.97;
        spectrum[i] = max(spectrum[i], val[i]);
    }
    
    float time = ofGetElapsedTimef();
    float dt = time - time0;
    dt = ofClamp(dt, 0.0, 0.1);
    time0 = time;
    
    Rad = ofMap( spectrum[ bandRad ], 1, 3, 800, 1600, true );
    Vel = ofMap(spectrum[ bandVel ], 0, 0.1, 0.05, 0.5);
    
    for(int j =0; j<n; j++){
        tx[j] += Vel * dt;
        ty[j] += Vel * dt;
        p[j].x = ofSignedNoise( tx[j] ) * Rad;
        p[j].y = ofSignedNoise( ty[j] ) * Rad;
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    for (int i = 0; i < nFrames; i ++) {
        txs[i]->draw(i* (frameW/2 + 5), frameH, frameW/2, frameH/2);
    }
    vid.draw(0, 0);
    ofEnableAlphaBlending();
    ofDisableAlphaBlending();
    ofDrawBitmapString("KEYS\n----\nspacebar: capture frame\ns: save gif", frameW+10, 20);


    
    
    //Draw spectrum
    ofSetColor( 0, 0, 0 );
    for (int i=0; i<N; i++) {
        //Draw bandRad and bandVel by black color,
        //and other by gray color
        if ( i == bandRad || i == bandVel ) {
            ofSetColor( 0, 0, 0 ); //Black color
        } else {
            ofSetColor( 128, 128, 128 ); //Gray color
        }
        ofRect( 10 + i * 5, 700, 3, -spectrum[i] * 100 );
    }
    
    
    //Draw cloud
    //Move center of coordinate system to the screen center
    //  ofPushMatrix();
    //  ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
    //Draw cloud's points
    //  ofSetColor( 230 );
    //  ofFill();
    //  for (int i=0; i<n; i++) {
   //       ofCircle( p[i], 2 );
   //   }
    //Draw lines between near points
   //   float dist = 60;    //Threshold parameter of distance
   //   for (int j=0; j<n; j++) {
   //       for (int k=j+1; k<n; k++) {
   //           if ( ofDist( p[j].x, p[j].y, p[k].x, p[k].y )
    //              < dist ) {
    //              ofLine( p[j], p[k] );
   //           } }
    //  }
    //Restore coordinate system
    ofPopMatrix();
    
    
    ofEnableDepthTest();
    
    ofBackgroundGradient( ofColor( 255 ), ofColor( 128 ) );
    ofPushMatrix();
   
    ofTranslate( ofGetWidth()/2, ofGetHeight()/2, 0 );
    
    vertices.resize( nVert );        //Set the array size
    
    for (int i=0; i<nTri; i++) {
        
        Rads = 200 + spectrum[i] * 2000;
        rad = 15 + spectrum[i] * 10;
        
        ofPoint center( cen1[i],cen2[i],cen3[i] );
        center.normalize();
        center *= Rads;

        for (int j=0; j<3; j++) {
            vertices[ i*3 + j ] =  center + ofPoint( ver1[i*3 + j], ver2[i*3 + j], ver3[i*3 + j]);
        } }
    
    
    
    //Calculate the rotation angle
    float time = ofGetElapsedTimef();    //Get time in seconds
    
    
    float angle = time * 10; //Compute angle. We rotate at speed
    //10 degrees per second
    ofRotate( angle, 0, 1, 0 );
    
    
    //Draw the triangles
    for (int i=0; i<nTri; i++) {
        ofSetColor( colors[i] );
        ofTriangle( vertices[ i*3 ],
                   //Set color
                   
                   vertices[ i*3 + 1 ],
                   vertices[ i*3 + 2 ] ); //Draw triangle
                   }
    ofPopMatrix();
                   
        //Restore the coordinate system
    }


void ofApp::onGifSaved(string &fileName) {
    cout << "gif saved as " << fileName << endl;
}

void ofApp::captureFrame() {
    gifEncoder.addFrame(
                        vid.getPixels(),
                        vid.getWidth(),
                        vid.getHeight(),
                        vid.getPixelsRef().getBitsPerPixel(),
                        .1f
                        );
    
    ofTexture * tx = new ofTexture();
    tx->allocate(frameW, frameH, GL_RGB);
    tx->loadData(vid.getPixels(), frameW, frameH, GL_RGB);
    txs.push_back(tx);
    
    nFrames++;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case ' ':
            captureFrame();
            break;
        case 's':
            cout <<"start saving\n" << endl;
            gifEncoder.save("test.gif");
            break;
        default:
            break;
    }

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
