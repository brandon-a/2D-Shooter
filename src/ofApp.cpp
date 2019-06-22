//Brandon Archbold
#include "ofApp.h"
#include "Emitter.h"
// Example code from Kevin M. Smith - CS 134 SJSU EmitterCollision example used as a starting point

//--------------------------------------------------------------
// Sets up the game, enables vertical sync to eliminate screen tearing, 
// sets a background color and sets up the game to display the title screen.

void ofApp::setup() {
	ofSetVerticalSync(true);
	screenSize = ofxLabel(ofParameter<std::string>("screenSize"), 1334, 750);
	ofBackground(0, 0, 0);
	gameRunning = false;
	gameOver = false;
}

// Once the spacebar is pressed this function is triggered, exiting the start screen and beginning the game
void ofApp::startGame() {
	if (!backgroundImage.load("images/StarBackground.png")) {		// loads the image of a spaceship
		cout << "Background image failed to load\n" << endl;
	}
	ofHideCursor();										// hides the mouse cursor for easy mouse control. Press h to reveal the mouse
	ship = new Emitter(new SpriteSystem());			// creates the spaceship
	ship->setChildSize(10, 10);						// sets the missile size to 10 x 10
	missileImgLoaded = false;							// variables to keep track of loaded images
	shipImgLoaded = false;

	if (shipImage.load("images/Ship.png")) {		// loads the image of a spaceship
		ship->setImage(shipImage);
		shipImgLoaded = true;
	}

	if (missileImage.load("images/laser.png")) {	// loads the projectile image
		ship->setChildImage(missileImage);
		missileImgLoaded = true;
	}

	if (!gameMusic.load("sounds/POL-galactic-trek-short.wav")) {	// game music from https://www.playonloop.com/2018-music-loops/galactic-trek/#free-download
		cout << "Game Music failed to load\n";
	}
	else {
		gameMusic.play();
		gameMusic.setLoop(true);
	}

	ship->setSoundFile("sounds/146725__leszek-szary__laser.wav");
	if(!explosionSound.load("sounds/435416__v-ktor__explosion13.wav"))
		cout << "Explosion sound failed to load\n";

	// sets up the emitter that spits out enemy ships
	spawnTop = new Emitter(new SpriteSystem());
	spawnTop->setChildSize(20, 20);
	spawnTop->drawable = true;
	spawnTop->haveImage = false;
	spawnTop->haveChildImage = true;
	spawnTop->drawable = false;
	spawnTop->setPosition(ofVec3f(ofGetWindowWidth() / 2, 0));	// adds the enemy ship image
	if (enemyImage.load("images/Enemy-Ship.png")) {
		spawnTop->setChildImage(enemyImage);
	}
	spawnTop->start();

	gravity = new GravityForce(ofVec3f(0, 10, 0));

	// sets up the particle system for the type 2 enemy ships
	enemyParticle.sys->addForce(gravity);
	enemyParticle.setVelocity(ofVec3f(-500, 0, 0));
	enemyParticle.setEmitterType(DirectionalEmitter);
	enemyParticle.setLifespan(20);
	enemyParticle.setRate(rate);
	enemyParticle.setParticleRadius(1);

	// spawns the type 2 ships
	spawnSides = new Emitter(new SpriteSystem());
	spawnSides->setChildSize(30, 30);
	spawnSides->drawable = true;
	spawnSides->haveImage = false;
	spawnSides->haveChildImage = true;
	spawnSides->drawable = false;
	spawnSides->setPosition(ofVec3f(ofGetWindowHeight() / 2, ofGetWindowWidth()));	// adds the enemy ship image
	if (enemyImage.load("images/enemy-2.png")) {
		spawnSides->setChildImage(enemyImage);
	}
	spawnSides->start();

	// particle physics for explosions
	impulse = new ImpulseRadialForce(10000.0);
	explosion.sys->addForce(impulse);
	explosion.setVelocity(ofVec3f(0, 0, 0));
	explosion.setOneShot(true);
	explosion.setEmitterType(RadialEmitter);
	explosion.setGroupSize(25);
	explosion.setLifespan(2);
	explosion.setVelocity(ofVec3f(velocity));
	explosion.setRate(rate);
	explosion.setParticleRadius(1);

	enemyVelVec = new ofVec3f(0, 200, 0);	// allows easy modification of velocity vector

	gui.setup();										// sets up the sliders for testing using the h key
	gui.add(rate.setup("rate", 5, 1, 20));				// sets up the slider for the rate of projectile emissions
	gui.add(life.setup("life", 5, .1, 10));				// sets up the slider for the lifespan of the projectiles
	gui.add(velocity.setup("velocity", ofVec3f(0, -1000, 0), ofVec3f(-2000, -2000, -2000), ofVec3f(2000, 2000, 2000))); // sets up the slider for the velocity of the projectiles
	gui.add(enemyRate.setup("enemy rate", 1, .01, 20));
	gui.add(enemyVelocity.setup("enemy velocity", *enemyVelVec, ofVec3f(-500, 0, -500), ofVec3f(500, 1000, 500)));

	bHide = true;		// variable to hide and show the panel

	ship->setPosition(ofVec3f(ofGetMouseX(), ofGetMouseY()));		// sets the initial position of the ship to be in the middle of the window
	ship->start();																		// enables the ship to begin shooting when the rate is above 0
}

//--------------------------------------------------------------
// updates the ship backend every time something changes
void ofApp::update() {
	if (gameRunning && !gameOver) {
		ship->setLifespan(life * 1000);    // convert to milliseconds 
		ship->setVelocity(ofVec3f(velocity));
		ship->update();

		spawnTop->setLifespan(10000);
		enemyVelVec->x = -200 + (std::rand() % (-200 - 200 + 1));
		if (score % 10 == 0) {
			*enemyVelVec = *enemyVelVec + ofVec3f(0, 2, 0);		// slowly increase the velocity of the enemies
			enemyRate = enemyRate * 1.3;
			score++;
		}
		enemyVelocity = *enemyVelVec;
		spawnTop->setVelocity(ofVec3f(enemyVelocity));
		spawnTop->setPosition(ofVec3f(rand() % (40 - ofGetWindowWidth() - 40 + 1),0));		// spawn enemies at random locations along the top of the screen
		spawnTop->setRate(enemyRate);
		spawnTop->update();

		spawnSides->setLifespan(10000);
		spawnSides->setVelocity(ofVec3f(-200, enemyRate, 0));
		spawnSides->setPosition(ofVec3f(ofGetWindowWidth(), rand() % (40 - ofGetWindowHeight() - 40 + 1)));		// spawn enemies at random locations along the side of the screen
		spawnSides->setRate(enemyRate);
		spawnSides->update();


		checkCollisions();
		explosion.update();
	}
	else if (gameOver) {
		ofShowCursor();
		gameMusic.stop();
	}
}


//--------------------------------------------------------------
void ofApp::draw(){
	if (gameRunning && !gameOver) {
		backgroundImage.draw(0, 0);
		ship->draw();		// draw the ship if the game is runnning otherwise display the start screen
		spawnTop->draw();
		spawnSides->draw();
		if (!bHide) {
			gui.draw();
		}
		string scoreText;
		scoreText += "Score: " + std::to_string(score);
		ofDrawBitmapString(scoreText, ofPoint(10, 20));
		explosion.draw();
	}
	else if (gameOver) {
		stringstream endScore;
		endScore << "Score: " << score << endl << "HighScore: " << highScore;
		ofDrawBitmapStringHighlight("Game Over!", 573, 399, ofColor(255, 255, 255), ofColor(0, 0, 0));
		ofDrawBitmapString(endScore.str(), 577, 450);
		ofDrawBitmapString("Press enter to play again", 518, 512);
	}
	else {		// display the start screen when the game starts
		ofDrawBitmapStringHighlight("Missile Blaster!", 573, 399, ofColor(255, 255, 255), ofColor(0, 0, 0));
		ofDrawBitmapString("Press the spacebar to continue", 518, 512);
	}

}

//  This is a simple O(M x N) collision check
//  For each missle check to see which invaders you hit and remove them
// example collision detection code by Kevin Smith
void ofApp::checkCollisions() {
	// find the distance at which the two sprites (missles and invaders) will collide
	// detect a collision when we are within that distance.
	//
	float collisionDist = ship->childHeight / 2 + spawnTop->childHeight / 2;

	// Loop through all the missiles, then remove any invaders that are within
	// "collisionDist" of the missiles.  the removeNear() function returns the
	// number of missiles removed.
	//
	for (int i = 0; i < ship->sys->sprites.size(); i++) {
		int point = spawnTop->sys->removeNear(ship->sys->sprites[i].trans, collisionDist);
		point += 2 * spawnSides->sys->removeNear(ship->sys->sprites[i].trans, collisionDist);
		int crash = spawnTop->sys->removeNear(ship->trans, collisionDist) + spawnSides->sys->removeNear(ship->trans, collisionDist);	// if the ship is hit end the game
		score += point;
		if (point > 0) {
			explosion.setPosition(ofVec3f(ship->sys->sprites[i].trans));		// explode with particle effects if an enemy ship is hit
			explosion.sys->reset();
			explosion.start();
			explosionSound.play();
			ship->sys->sprites.erase(ship->sys->sprites.begin() + i);
		}
		if (crash > 0)	// end the game if the ship is hit
			gameOver = true;
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
	if(gameRunning)
		ship->setRate(rate);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if(gameRunning)
		ship->setRate(0);
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
	case OF_KEY_RETURN:
		if (gameOver) {
			gameOver = false;
			if (score > highScore)
				highScore = score;
			score = 0;
			startGame();
		}
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

