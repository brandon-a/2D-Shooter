#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Sprite.h"


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
		
		Emitter *emitter; 
		Sprite collider;

		ofImage defaultImage;
		ofVec3f mouse_last;
		bool imageLoaded;

		bool bHide;
		bool gameRunning;

		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		ofxLabel screenSize;

		ofxPanel gui;
};
