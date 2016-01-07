//
//  animationController.h
//  karmaMapper
//
//  Created by Daan de Lange on 12/03/2015.
//
//	This class handles the animation.
//	It links the shapes with effects, controls animation and basically links everything together.
//

#pragma once


#include "ofMain.h"
#include "KMSettings.h"
#include "shapes.h"
#include "effects.h"
#include "shapesDB.h"
//#include "ofxGui.h"
#include "ofxImGui.h"
#include "animationParamsServer.h"

#include "OSCRouter.h"
#include "mirReceiver.h"
#include "durationReceiver.h"
//#include "fboRecorder.h"

#ifdef TARGET_OSX
	//	#include "ofxSyphon.h"
#endif

// todo: an overall mask that hides any unwanted projection zones (could be done by an effect too)
// Ensure shape names are always UNIQUE

class animationController {
	// let ImGui control app variables
	friend class ofxImGui;
	
public:
	animationController( shapesDB& _scene );
	~animationController();
	// fix for no default constructor
	animationController& operator=(const animationController& crap) { return *this; }
	
	// basic functions
	bool start();
	bool stop();
	bool isEnabled() const;

	// load & save
	bool loadConfiguration(const string& _file = "");
	bool loadLastConfiguration();
	bool saveConfiguration(const string& _fileName = "");
	string loadedConfiguration;
	
	// event handlers
	void update( ofEventArgs& event );
	void draw( ofEventArgs& event );
	void _keyPressed( ofKeyEventArgs& e );
	void _mousePressed( ofMouseEventArgs& e );
	
	// menu listeners
	void showShapeLoadMenu( );
	void showSaveDialog( );
	void setFullScreen( bool _fullScreen );
	void setShowMouse( bool _showMouse );
	void showAnimationsGui( bool& _b );
	//void enableAnimations( bool & _on );
	
protected:
	// app variables
	bool bIsFullScreen;
	bool bShowMouse;
	bool bShowGui;
	bool bGuiShowAnimParams;
	
	// gui
	ofxImGui gui;
	
	// effects stuff
	bool isEffectsIndex(int i);
	vector<basicEffect*> effects;
	
	animationParamsServer animationParams;
	
	shapesDB& scene;
	
	// OSC
	OSCRouter oscRouter;
	mirReceiver mirOSCReceiver;
	//durationReceiver durationOSCReceiver;
	
	// Video recording
	//fboRecorder recorder;
	
private:
	bool bEnabled;
	
#ifdef TARGET_OS_MAC
        //ofxSyphonServer syphonTexture;
#endif
};


// GUI translations

// Keyboard Shordcuts
#define GUIMouseHide	("Hide Cursor")
#define GUIGuiToggle	("Toggle GUI")
#define GUIselected		("Toggle GUI")


// Shapes
#define GUIShapesInfo		("Scene Information")
#define GUILoadScene		("Load Shapes...")
#define GUILoadedScene		("") // empty for having more gui space
#define GUIShapesNumShapes	("# Shapes:\t")


// Effects
#define GUIEffectsTitle			("Effects Information")
#define GUIToggleFullScreen		("Full Screen")
#define GUIEffectsViewParams	("Show Animation Parameters")


