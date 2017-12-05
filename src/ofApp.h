#pragma once

#include "ofMain.h"
#include "StringUtil.h"
#include "Parameter.h"


#define CaptainPin 20
#define LaserPin 18
#define ATalkPin 5
#define BTalkPin 14
#define ProxPin 21

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

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


		//sound
		vector<ofSoundPlayer>  _sound;
		vector<int> _status;
		vector<int> _prox_diff;

		ofSoundPlayer _bgm;
		int _msound;

		vector<ofSoundPlayer>  _captain;
		vector<ofSoundPlayer>  _laser;
		vector<ofSoundPlayer> _atalk;
		vector<ofSoundPlayer> _btalk;

		void toggleBgm();

		int _play_captain;
		int _play_laser;
		int _play_atalk;
		int _play_btalk;

		//serial
		ofSerial* _serial;
		
		void initSerial();
		void checkSerial();
		void checkSerial(int index_);


		void updateSerial();
		void touchSound(int index_);
		void releaseSound(int index_);
		void updateProxSound(int index_,int val_);

		//Parameter
		GlobalParam* _global;
};
