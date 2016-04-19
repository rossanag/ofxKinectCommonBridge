#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	kinect.initSensor(0);
	//kinect.initColorStream(640, 480);
	//kinect.initDepthStream(320, 240);
	kinect.initFaceTracking();

	kinect.start();
}

void ofApp::exit()
{
	kinect.stop();
}

//--------------------------------------------------------------
void ofApp::update(){
	kinect.update();
	if(kinect.isFaceNew()) {
		face = kinect.getFaceData();
	}
}

//--------------------------------------------------------------
void ofApp::draw()
{

	ofBackground(100, 100, 100);
	ofSetColor(0, 255, 0 );

	ofNoFill();
	ofRect( face.rect ); // draw the bounding box of the face

	ofSetColor(255, 255, 255 );
	face.mesh.drawWireframe(); // draw the mesh of the face
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
