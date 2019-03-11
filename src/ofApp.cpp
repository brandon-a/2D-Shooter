//Brandon Archbold
#include "ofApp.h"
#include "Emitter.h"
// Example code from Kevin M. Smith - CS 134 SJSU EmitterCollision example used as a starting point

//--------------------------------------------------------------
// Sets up the game, enables vertical sync to eliminate screen tearing, 
// sets a background color and sets up the game to display the title screen.
void ofApp::setup(){
	ofSetVerticalSync(true);
    screenSize = ofxLabel(ofParameter<std::string>("screenSize"),1334, 750);
	ofBackground(0, 0, 0);
	gameRunning = false;
}

// Once the spacebar is pressed this function is triggered, exiting the start screen and beginning the game
void ofApp::startGame() {
	ofHideCursor();										// hides the mouse cursor for easy mouse control. Press h to reveal the mouse
	ship = new Emitter(new SpriteSystem());			// creates the spaceship
	ship->setChildSize(10, 10);						// sets the missile size to 10 x 10
	missileImgLoaded = false;							// variables to keep track of loaded images
	shipImgLoaded = false;

	if (shipImage.loadImage("images/Ship.png")) {		// loads the image of a spaceship
		ship->setImage(shipImage);
		shipImgLoaded = true;
	}

	if (missileImage.loadImage("images/laser.png")) {	// loads the projectile image
		ship->setChildImage(missileImage);
		missileImgLoaded = true;
	}

	ship->setSoundFile("sounds/pew.mp3");

	spawnTop = new Emitter(new SpriteSystem());
	spawnTop->setChildSize(20, 20);
	spawnTop->drawable = true;
	spawnTop->haveImage = false;
	spawnTop->haveChildImage = false;
	spawnTop->setPosition(ofVec3f(ofGetWindowWidth() / 2, 0));
	spawnTop->start();

	gui.setup();										// sets up the sliders for testing using the h key
	gui.add(rate.setup("rate", 5, 1, 20));				// sets up the slider for the rate of projectile emissions
	gui.add(life.setup("life", 5, .1, 10));				// sets up the slider for the lifespan of the projectiles
	gui.add(velocity.setup("velocity", ofVec3f(0, -1000, 0), ofVec3f(-2000, -2000, -2000), ofVec3f(2000, 2000, 2000))); // sets up the slider for the velocity of the projectiles

	bHide = true;		// variable to hide and show the panel

	ship->setPosition(ofVec3f(ofGetMouseX(), ofGetMouseY()));		// sets the initial position of the ship to be in the middle of the window
	ship->start();																		// enables the ship to begin shooting when the rate is above 0
}

//--------------------------------------------------------------
// updates the ship backend every time something changes
void ofApp::update() {
	if (gameRunning) {
		ship->setLifespan(life * 1000);    // convert to milliseconds 
		ship->setVelocity(ofVec3f(velocity));
		ship->update();

		spawnTop->setLifespan(10000);
		spawnTop->setVelocity(ofVec3f(0, 200, 0));
		spawnTop->update();
	}
}


//--------------------------------------------------------------
void ofApp::draw(){
	if (gameRunning) {
		ship->draw();		// draw the ship if the game is runnning otherwise display the start screen
		spawnTop->draw();
		if (!bHide) {
			gui.draw();
		}
	}
	else {		// display the start screen when the game starts
		ofDrawBitmapStringHighlight("Missile Blaster!", 573, 399, ofColor(255, 255, 255), ofColor(0, 0, 0));
		ofDrawBitmapString("Press the spacebar to continue", 518, 512);
	}
}

//--------------------------------------------------------------
// Allows easy control of the ship by essentially turning the mouse into the ship
void ofApp::mouseMoved(int x, int y ){
    moveSprite(ofVec3f(x, y, 0));
}

//--------------------------------------------------------------
// Allows easy control of the ship by essentially turning the mouse into the ship,
// ensures that it still works if the mouse is clicked
void ofApp::mouseDragged(int x, int y, int button){
    moveSprite(ofVec3f(x, y, 0));
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

void ofApp::keyPressed(int key) {
	switch (key) {
	case ' ':
        if(!gameRunning){			// if the game is running start shooting,
            gameRunning = true;		// if not start the game
            startGame();
            ship->setRate(0);
        }
        else{
            ship->setRate(rate);
        }
		break;
	case 'H':
	case 'h':
		bHide = !bHide;		// hide and show the panel
		if (bHide)
			ofHideCursor();	// hide cursor when panel is hidden
		else
			ofShowCursor();	// show cursor when panel is visible
		break;
    case OF_KEY_LEFT:
            moveSprite(MoveLeft);
            cout << "left" << endl;
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
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
        case ' ':
            if(gameRunning){
                ship->setRate(0);		// stops the ship from shooting when the spacebar is released
            }
            break;
	}
}

// function to check bounds and allow movement with the mouse
void ofApp::moveSprite(ofVec3f pos){
    if(gameRunning && pos.x >= 0 && pos.x <= 1334 && pos.y >= 0 && pos.y <= 750){
        ship->trans = pos;
    }
}
// function to check bounds and allow movement with arrow keys of the ship
void ofApp::moveSprite(MoveDir dir){
    switch(dir){
        case MoveLeft: if(ship->trans.x >= 0) ship->trans += ofVec3f(-10, 0, 0);
            break;
        case MoveRight: if(ship->trans.x <= 1334) ship->trans += ofVec3f(10, 0, 0);
            break;
        case MoveUp: if(ship->trans.y >= 0) ship->trans += ofVec3f(0, -10, 0);
            break;
        case MoveDown: if(ship->trans.y <= 750) ship->trans += ofVec3f(0, 10, 0);
            break;
        case MoveStop: ship->trans += ofVec3f(0, 0, 0);
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

