#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetVerticalSync(true);
	ofLogToFile("kkplaygnd_"+ofGetTimestampString()+".log",true);


	_global=new GlobalParam();

	_bgm.load("sound/BASE 1129.wav");
	_bgm.setLoop(true);
	_bgm.setVolume(0.7);
	_bgm.setMultiPlay(true);
	_bgm.play();

	/*string keyname[13]={"kb_violin_Do","kb_violin_Do#","kb_violin_Re","kb_violin_Re#","kb_violin_Mi",
						"kb_violin_Fa","kb_violin_Fa#","kb_violin_So","kb_violin_So#","kb_violin_La",
						"kb_violin_La#","kb_violin_Ti","kb_violin_HiDo"};*/
	/*string keyname[13]={"kbtest Do","kbtest Do#","kbtest Re","kbtest Re#","kbtest Mi",
		"kbtest Fa","kbtest Fa#","kbtest So","kbtest So#","kbtest La",
		"kbtest La#","kbtest Ti","kbtest Do"};*/
	string frontname_="FM/kb_FM1_";
	string keyname[13]={"Do","Do#","Re","Re#","Mi",
		"Fa","Fa#","So","So#","La",
		"La#","Ti","HiDo"};
	for(int i=0;i<13;++i){
		ofSoundPlayer p_;
		p_.load("sound/keyboard/"+frontname_+keyname[i]+".wav");
		_sound.push_back(p_);

		if(i==4 || i==12){
			ofSoundPlayer g_;
			g_.load("sound/synth/1. Captain/captain4.wav");
			_sound.push_back(g_);
		}
	}
	
	vector<ofSoundPlayer> synth_;
	string synthname[8]={"6. PIPI","4. chuchuchu","5. Flying","Laser1","8. Shoooo","captain1","7. RingTone","3. AlienTalk"};
	for(int i=0;i<8;++i){
		ofSoundPlayer p_;
		p_.load("sound/synth/"+synthname[i]+".wav");
		_sound.push_back(p_);
	}	

	string drumname[5]={"3. RIm","4. snare","1. Hihat","2. Kick","5. Tom"};
	for(int i=0;i<5;++i){
		ofSoundPlayer p_;
		p_.load("sound/drum/"+drumname[i]+".wav");
		_sound.push_back(p_);
	}

	_msound=_sound.size();
	_sound[ProxPin].setLoop(true);
	_sound[ProxPin].play();
	_sound[ProxPin].setVolume(0);

	//PIPI
	_sound[LoopPin].setLoop(true);


	for(int i=1;i<5;++i){
		ofSoundPlayer p_;
		p_.load("sound/synth/1. Captain/captain"+ofToString(i)+".wav");
		_captain.push_back(p_);
	}
	for(int i=1;i<4;++i){
		ofSoundPlayer p_;
		p_.load("sound/synth/2. Laser/Laser"+ofToString(i)+".wav");
		_laser.push_back(p_);
	}

	string aname[4]={"A_humming 03","A_kiss","A_says_yo","A_whats_going_on"};
	for(int i=0;i<4;++i){
		ofSoundPlayer p_;
		p_.load("sound/talk/"+aname[i]+".wav");
		_atalk.push_back(p_);
	}

	string bname[4]={"B_uh_ohh_15415","B_will_you_shut_up_13100"};
	for(int i=0;i<2;++i){
		ofSoundPlayer p_;
		p_.load("sound/talk/"+bname[i]+".wav");
		_btalk.push_back(p_);
	}


	for(int i=0;i<_msound;++i){
		_status.push_back(0);
		_prox_diff.push_back(0);
	}

	initSerial();


	_itest=0;
	


}
void ofApp::checkSerial(int index_){
	

	int mserial=_serial[index_].getDeviceList().size();
	if(index_>=mserial){
		ofLog()<<"no such port"<<_global->_port[index_]<<" !";
		return;
	}

	if(_serial[index_].isInitialized()) _serial[index_].close();
	_serial[index_].setup(_global->_port[index_],19200);

}
void ofApp::checkSerial(){
	for(int i=0;i<GlobalParam::MPort;++i) checkSerial(i);
}
void ofApp::initSerial(){

	_serial=new ofSerial[GlobalParam::MPort];

	for(int i=0;i<GlobalParam::MPort;++i){
		_serial[i].setup(_global->_port[i],19200);
		if(_serial[i].isInitialized()) ofLog()<<"set up "<<_global->_port[i]<<" success!!";
		else  ofLog()<<"set up "<<_global->_port[i]<<" fail!!";
		
	}
}
//--------------------------------------------------------------
void ofApp::update(){
	updateSerial();
	ofSoundUpdate();


	ofSetWindowTitle(ofToString(ofGetFrameRate()));

}

void ofApp::updateSerial(){
	for(int i=0;i<GlobalParam::MPort;++i){
		if(!_serial[i].isInitialized() || !_serial[i].available()) continue;

			vector<string> val=readSerialString(_serial[i],'#');
			if(val.size()<2) return;

			//ofLog()<<"serial "<<i<<" read: "<<ofToString(val)<<endl;
			
			int index_=ofToInt(val[1]);
			//if(index_>=_global->_msound[i]) continue;

			index_+=_global->_sstart[i];
			

			if(val[0]=="T") touchSound(index_);						
			else if(val[0]=="R") releaseSound(index_);						
			else if(val[0]=="P"){
				if(val.size()>2) updateProxSound(index_,ofToInt(val[2]));
			}
	}
}

void ofApp::touchSound(int index_){

	//if(index_>_sound.size()) return;

	if(index_==CaptainPin){
		if(!_captain[_play_captain].isPlaying()){
			_play_captain=ofRandom(_captain.size());
			_captain[_play_captain].play();
		}
	}else if(index_==LaserPin){
		_play_laser=ofRandom(_laser.size());
		_laser[_play_laser].play();
	}else if(index_==ATalkPin){
		if(!_atalk[_play_atalk].isPlaying()){
			_play_atalk=ofRandom(_atalk.size());
			_atalk[_play_atalk].play();
		}
	}else if(index_==BTalkPin){
		if(!_btalk[_play_btalk].isPlaying()){
			_play_btalk=ofRandom(_btalk.size());
			_btalk[_play_btalk].play();
		}
	}else{
		if(index_!=ProxPin){
			_sound[index_].stop();	
			_sound[index_].play();	
			_sound[index_].setPosition(0);
		}
	}

	_status[index_]=1;

	ofLog()<<ofGetTimestampString()<<"  --touch "<<index_;
}

void ofApp::releaseSound(int index_){

	if(index_>_sound.size()) return;

	//if(index_==CaptainPin) _captain[_play_captain].stop();
	//else if(index_==LaserPin) _laser[_play_laser].stop();
	//else if(index_==ATalkPin) _atalk[_play_atalk].stop();
	//else if(index_==BTalkPin) _btalk[_play_btalk].stop();
	if(index_<15) _sound[index_].stop();
	if(index_==15 || index_==16 || index_==17 || index_==22) _sound[index_].stop();
	
	_status[index_]=0;
	ofLog()<<ofGetTimestampString()<<"  --release "<<index_;
}

void ofApp::updateProxSound(int index_,int val_){
	
	if(_status[index_]==1){
		_sound[index_].setVolume(1.0);
		_sound[index_].setSpeed(1.0);
		return;
	}
	_prox_diff[index_]=val_;

	if(val_>_global->_prox_low_thres){
		_sound[index_].setVolume(1.0);
		_sound[index_].setSpeed(ofClamp(ofMap(val_,_global->_prox_low_thres,_global->_prox_high_thres,0.1,1.0),0.1,1.0));				
	}else{
		_sound[index_].setVolume(0);
	}

}


//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);

	ofPushMatrix();

	float h=(float)ofGetHeight()*.8/(float)GlobalParam::MPort;
	float r=h*.3;

	ofTranslate(r*1.2,r*2);

	int count_=0;
	for(int i=0;i<GlobalParam::MPort;++i){
		
		ofSetColor(255);
		ofFill();
		ofDrawBitmapString(_global->_port[i],0,-r*1.2);

		ofPushMatrix();
		for(int j=0;j<_global->_msound[i];++j){

			float r_=r*ofMap(_prox_diff[count_],20,200,1.0,5.0);

			if(_status[count_]) ofSetColor(255);
			else ofSetColor(150);		

			ofDrawCircle(r*j,0,r_);


			if(_itest==count_){
				ofPushStyle();
					ofSetColor(0,0,255);
					ofNoFill();
					ofDrawCircle(r*j,0,r_);
				ofPopStyle();			
			}

			count_++;
			ofTranslate(r*1.2,0);
		}
		ofPopMatrix();


		ofTranslate(0,h);
	}
	ofPopMatrix();

	stringstream hint_;
	hint_<<" 1-4   "<<"check serial port"<<endl
		 <<" space "<<"toggle bgm"<<endl
		 <<" t     "<<"test sound"<<endl
		 <<" r     "<<"move test index"<<endl;
	ofDrawBitmapString(hint_.str(),20,ofGetHeight()-80);


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case '1':
		case '2':
		case '3':
		case '4':
			checkSerial(key-'1');
			break;
		case ' ':
			toggleBgm();
			break;
		case 't':
			//sound test
			ofLog()<<"sound test!";			
			if(_status[_itest]==1) releaseSound(_itest);
			else touchSound(_itest);
			_itest=(_itest+1)%_msound;
			break;
		case 'r':
			//sound test
			_itest=(_itest+1)%_msound;
			break;
	}
}

void ofApp::toggleBgm(){
	auto player=_bgm.getPlayer();
	auto fmodPlayer=dynamic_pointer_cast<ofFmodSoundPlayer>(player);
	
	if(fmodPlayer->bPaused){
		_bgm.setPaused(false);
	}else{
		_bgm.setPaused(true);
	}			
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
