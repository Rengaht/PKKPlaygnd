#pragma once
#ifndef PARMAMETR_H
#define PARAMETER_H


#include "ofMain.h"
#include "ofxXmlSettings.h"

class GlobalParam{

	static GlobalParam* _instance;	
	
public:
	static int MPort;
	static string ParamFilePath;	
    
	vector<string> _port;
    vector<int> _msound;
    vector<int> _sstart;

	int _prox_low_thres;
	int _prox_high_thres;


	GlobalParam(){
		readParam();

	}
	static GlobalParam* GetInstance(){
		if(!_instance)
			_instance=new GlobalParam();
		return _instance;
	}
	void readParam(){


		ofxXmlSettings _param;
		bool file_exist=true;
		if(_param.loadFile(ParamFilePath) ){
			ofLog()<<"mySettings.xml loaded!";
		}else{
			ofLog()<<"Unable to load xml check data/ folder";
			file_exist=false;
		}


		_port.clear();
		_msound.clear();
		_sstart.clear();

        int mport=_param.getNumTags("PORT");
		for(int i=0;i<mport;++i){
			_port.push_back(_param.getValue("PORT","",i));
			_msound.push_back(ofToInt(_param.getAttribute("PORT","MSOUND","",i)));
			_sstart.push_back(ofToInt(_param.getAttribute("PORT","START","",i)));
		}

		_prox_low_thres=_param.getValue("PROX_LOW_THRES",0);
		_prox_high_thres=_param.getValue("PROX_HIGH_THRES",0);

		if(!file_exist) saveParameterFile();

	
	}
	void saveParameterFile(){


		ofxXmlSettings _param;
        
		while(_port.size()<MPort){
			_port.push_back("_null");
			_msound.push_back(0);
			_sstart.push_back(0);
		}
        for(int i=0;i<_port.size();++i){
			_param.setValue("PORT",_port[i],i);
			_param.setAttribute("PORT","MSOUND",ofToString(_msound[i]),i);
			_param.setAttribute("PORT","START",ofToString(_sstart[i]),i);
		}

		_param.setValue("PROX_LOW_THRES",_prox_low_thres);
		_param.setValue("PROX_HIGH_THRES",_prox_high_thres);

		_param.save(ParamFilePath);


	}


};





#endif
