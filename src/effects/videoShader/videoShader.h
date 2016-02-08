//
//  videoShader.h
//  karmaMapper
//
//  Created by Daan de Lange on 10/10/15.
//
//
//

#pragma once

#include "ofMain.h"
#include "shapes.h"
#include "shaderEffect.h"
#include "animationParams.h"
#include "mirReceiver.h"

struct animationParams;

// todo: make video decoding threaded

enum videoMode {
	VIDEO_MODE_FILE = 0, // read image from file
	VIDEO_MODE_SYPHON = 1 // read movie from file
};

// Important: lock() when accessing player or bUseThreadedFileDecoding

class videoShader : public shaderEffect, public ofThread {
	
public:
	// constructors
	videoShader();
	~videoShader();
	
	// global effect functions
	bool initialise(const animationParams& params);
	bool render(const animationParams& params);
	void update(const animationParams& params);
	void reset();
	
	// #########
	// GUI STUFF
	virtual bool printCustomEffectGui();
	
	// #########
	// LOAD & SAVE FUNCTIONS
	virtual bool saveToXML(ofxXmlSettings& xml ) const;
	virtual bool loadFromXML(ofxXmlSettings& xml);
	
	// controller functions
	bool randomizePresets();
	
	// #########
	// videoEffect FUNCTIONS
	void setVideoMode(const enum videoMode& mode);
	bool loadVideoFile( const string &_path);
	void setUseThread( const bool& _useThread );
	
protected:
	videoMode videoMode;
	
	float playBackSpeed;
	string videoFile;
	
	bool bUseThreadedFileDecoding;
	ofThreadChannel<ofPixels> images_to_update;
	virtual void threadedFunction();
	
	ofVideoPlayer player;
	
private:
	
	
};

#define GUIvideoShaderPanel "Video Shader"