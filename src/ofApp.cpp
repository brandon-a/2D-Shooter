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
    screenSize = ofxLabel(ofParameter<std::string>("screenSize"),1334, 750);
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
	gui.add(rate.setup("rate", 5, 1, 10));
	gui.add(life.setup("life", 5, .1, 10));
	gui.add(velocity.setup("velocity", ofVec3f(0, -700, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));

	bHide = true;

	emitter->setPosition(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0));
	emitter->start();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (gameRunning) {
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
    moveSprite(ofVec3f(x, y, 0));
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    moveSprite(ofVec3f(x, y, 0));
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
        if(!gameRunning){
            gameRunning = true;
            startGame();
            emitter->setRate(0);
        }
        else{
            emitter->setRate(rate);
        }
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
    case OF_KEY_LEFT:
            moveSprite(MoveLeft);
            break;
    case OF_KEY_RIGHT:
            moveSprite(MoveRight);
            break;
    case OF_KEY_UP:
            moveSprite(MoveUp);
            break;
    case OF_KEY_DOWN:
            moveSprite(MoveDown);
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
        case ' ':
            if(gameRunning){
                emitter->setRate(0);
            }
            break;
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


void ofApp::moveSprite(ofVec3f pos){
    if(gameRunning && pos.x >= 0 && pos.x <= 1334 && pos.y >= 0 && pos.y <= 750){
        emitter->trans = pos;
    }
}

void ofApp::moveSprite(MoveDir dir){
    switch(dir){
        case MoveLeft: if(emitter->trans.x >= 0) emitter->trans += ofVec3f(-10, 0, 0);
            break;
        case MoveRight: if(emitter->trans.x <= 1334) emitter->trans += ofVec3f(10, 0, 0);
            break;
        case MoveUp: if(emitter->trans.y >= 0) emitter->trans += ofVec3f(0, -10, 0);
            break;
        case MoveDown: if(emitter->trans.y <= 750) emitter->trans += ofVec3f(0, 10, 0);
            break;
        case MoveStop: emitter->trans += ofVec3f(0, 0, 0);
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

