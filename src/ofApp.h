//Brandon Archbold
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Sprite.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "TransformObject.h"
#include "Particle.h"


typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;



class Emitter;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void checkCollisions();
		void startGame();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void moveSprite(ofVec3f pos);
        void moveSprite(MoveDir dir);
		
		Emitter *ship;
		Emitter *spawnTop;
		Emitter *spawnSides;
		ParticleEmitter enemyParticle;

		ParticleEmitter explosion;
		ImpulseRadialForce* impulse;
		GravityForce* gravity;

		ofSoundPlayer explosionSound;
		ofSoundPlayer gameMusic;

		ofImage backgroundImage;
		ofImage missileImage;
		ofImage shipImage;
		ofImage enemyImage;
		ofImage enemyImage2;
		ofVec3f mouse_last;
		ofVec3f* enemyVelVec;
		bool missileImgLoaded;
		bool shipImgLoaded;

		bool bHide;
		bool gameRunning;
		bool gameOver;

		int score = 0;
		int highScore = 0;

		ofxVec3Slider enemyVelocity;
		ofxFloatSlider enemyRate;
		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		ofxLabel screenSize;

		ofxPanel gui;
};
