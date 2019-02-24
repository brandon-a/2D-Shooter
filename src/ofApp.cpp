#include "ofApp.h"
#include "Emitter.h"
//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Emitter" class to emit Sprites
// and set them in motion. The concept of an "Emitter" is taken from particle
// systems (which we will cover next week).
//
//  1) Creates a simple Emitter  that you can drag around (without images)
//  2) Creates an Sprite to Collide with
//
//  requires ofxGUI add-in for sliders for changing parameters
// 
//
//
//  Kevin M. Smith - CS 134 SJSU

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0, 0, 0);
	gameRunning = false;
}

void ofApp::startGame() {
	emitter = new Emitter(new SpriteSystem());
	emitter->setChildSize(10, 10);
	collider.setPosition(ofVec3f(800, 800));
	collider.width = 20;
	collider.height = 20;
	imageLoaded = false;

	/*if (defaultImage.loadImage("C:\\Users\\Brandon\\source\\of_v0.10.1_vs2017_release\\apps\\myApps\\2D-Shooter\\bin\\data\\images\\small.drone.PNG")) {
		emitter->setChildImage(defaultImage);
		imageLoaded = true;
	}*/

	gui.setup();
	gui.add(rate.setup("rate", 1, 1, 10));
	gui.add(life.setup("life", 2, .1, 10));
	gui.add(velocity.setup("velocity", ofVec3f(100, 100, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));

	bHide = true;

	emitter->setPosition(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0));
	emitter->start();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (gameRunning) {
		emitter->setRate(rate);
		emitter->setLifespan(life * 1000);    // convert to milliseconds 
		emitter->setVelocity(ofVec3f(velocity));
		emitter->update();

		checkCollisions();
	}
}


//--------------------------------------------------------------
void ofApp::draw(){
	if (gameRunning) {
		emitter->draw();
		collider.draw();

		if (!bHide) {
			gui.draw();
		}
	}
	else {
		ofDrawBitmapStringHighlight("Missile Blaster!", 573, 399, ofColor(255, 255, 255), ofColor(0, 0, 0));
		ofDrawBitmapString("Press the spacebar to continue", 518, 512);
	}
}

void ofApp::checkCollisions() {
	if (gameRunning) {
		// remove all sprites emitted within a radius equal to the max
		// distance sprite can travel in one frame.
		//
		float dist = emitter->maxDistPerFrame();
		collider.width = dist;
		collider.height = dist;
		emitter->sys->removeNear(collider.trans, emitter->maxDistPerFrame());
	}
}


//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//	cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (gameRunning) {
		ofPoint mouse_cur = ofPoint(x, y);
		ofVec3f delta = mouse_cur - mouse_last;
		emitter->trans += delta;
		mouse_last = mouse_cur;
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	mouse_last = ofPoint(x, y);
	cout << x << ", " << y << endl;
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

void ofApp::keyPressed(int key) {
	switch (key) {
	case ' ':
		gameRunning = true;
		startGame();
		break;
	case 'C':
	case 'c':
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		bHide = !bHide;
		break;
	case 'r':
		break;
	case 's':
		break;
	case 'u':
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case OF_KEY_LEFT:
	case OF_KEY_RIGHT:
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	}
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

