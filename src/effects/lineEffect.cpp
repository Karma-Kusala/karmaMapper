//
//  lineEffect.cpp
//  karmaMapper
//
//  Created by Daan de Lange on 12/3/14.
//
//  - - - -
//
//  Parent class for all effects.
//  Implements some standard methods for overall usage.
//

#include "lineEffect.h"

lineEffect::lineEffect(){
	//basicEffect::basicEffect();
	ofScopedLock lock(effectMutex);
	
	// effectType must match the class name
	effectType = "lineEffect";
	
	lines.clear();
	/*for(int i=0; i<shapes.size()*5; i++){
		
		lines.push_back( getRandomLine() );
	 }*/
	
	fromShape=NULL;
	toShape=NULL;
	
	lines.clear();
	tempoCalls=0;
}

lineEffect::~lineEffect(){
	ofRemoveListener(durationReceiver::durationFloatEvent, this, &lineEffect::floatListener);
	//ofRemoveListener(ofx::AbletonLiveSet::EventHandler::noteEvent, this, &lineEffect::noteEventListener);
	ofRemoveListener(mirReceiver::mirTempoEvent, this, &lineEffect::tempoEventListener);
	ofRemoveListener(mirReceiver::mirOnSetEvent, this, &lineEffect::onSetEventListener);
}

bool lineEffect::initialise(){
	
	basicEffect::initialise();
	
	// tmp
	ofAddListener(durationReceiver::durationFloatEvent, this, &lineEffect::floatListener);
	ofAddListener(mirReceiver::mirTempoEvent, this, &lineEffect::tempoEventListener);
	ofAddListener(mirReceiver::mirOnSetEvent, this, &lineEffect::onSetEventListener);
	//ofAddListener(ofx::AbletonLiveSet::EventHandler::noteEvent, this, &lineEffect::noteEventListener);
	
	
	// todo: this could be set to the basicShape::getBoundingBox() size for better performance
	// tmp
	//renderer.allocate( ofGetCurrentRenderer()->getViewportWidth(), ofGetCurrentRenderer()->getViewportHeight(), GL_RGBA, 8); // lower this to get better FPS
	renderer.allocate( 1500, 1200, GL_RGBA, 8);
	renderer.begin();
	ofClear(0,0,0,0); // fill with total invisibility! >D
	//ofEnableSmoothing(); // enables smooth lines (makes no difference)
	renderer.end();
	
	isInitialised = renderer.isAllocated();
	
	return isInitialised;
}

// update --> animation
// overrule this function with your own
bool lineEffect::render(){
	effectMutex.lock();
	
	ofFill();
	//basicEffect::render();
	
	if( shapes.size()==0 ){
		effectMutex.unlock();
		return false;
	}
	effectMutex.unlock();
	
	// partial frame buffering
	if(renderer.isAllocated()){
		renderer.begin();
		
		// tmp (to re-enable)
		// fade FBO alpha over time
		//ofEnableAlphaBlending(); // tmp
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_SUBTRACT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		ofSetColor(0,0,0,50); // */
		
		//ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
		//ofSetColor(0,0,0, (mirReceiver::mirCache.pitch/1024+2)*255-2); // multiplies current buffer color by this one
		ofFill();
		ofDrawRectangle(0,0,renderer.getWidth(), renderer.getHeight());
		ofDisableBlendMode(); // resets blending modes manually enabled above
	}
	
	effectMutex.lock();
	ofSetColor(255);
	for(std::list<lineEffectLine>::iterator it=lines.begin(); it!= lines.end(); ++it){
		(*it).render();
	}
	effectMutex.unlock();
	
	if(renderer.isAllocated()){
		renderer.end();
		renderer.draw(0,0);
	}
}

void lineEffect::update(){
	basicEffect::update();
	
	ofScopedLock lock(effectMutex);
	if(!isReady()) return;
	
	// initial shape setup. Dirty to do it here. // todo
	if(fromShape==NULL) fromShape = shapes[ round( ofRandom(-0.49f, shapes.size()-0.51f) )];
	if(toShape==NULL) toShape = shapes[ round( ofRandom(-0.49f, -0.51f+shapes.size()) )];
	
	// tmp
	/*if(lines.size()==0){
	basicShape* current;
	basicShape* prev=shapes[ round( ofRandom(-0.49f, shapes.size()-0.51f) )];
	for(int i=0; i<shapes.size(); i++){
		current=shapes[ round( ofRandom(-0.49f, shapes.size()-0.51f) )];
		for(int ii=0; ii<5;ii++){
			lines.push_back(getRandomLine( current, prev));
		}
		prev=shapes[ round( ofRandom(-0.49f, shapes.size()-0.51f) )];
	}
	}*/
	
	
	
	// add lines ?
	//if(lines.size() < shapes.size()*30) lines.push_back( getRandomLine() );
	
	// check for dead lines
	for(std::list<lineEffectLine>::reverse_iterator it=lines.rbegin(); it!= lines.rend(); it--){
		
		//lineEffectLine bla = *it;
		
		// replace dead one with new one
		if( !(*it).isAlive() ) {
			//*it = getRandomLine();
			it++;
			it= std::list<lineEffectLine>::reverse_iterator( lines.erase( it.base() ));
			it--;
		}
	}
}

// resets all values
// overrule this function with your own.
void lineEffect::reset(){
	// do everything the basicEffect does
	basicEffect::reset();
	
	ofScopedLock lock(effectMutex);
	totalLinesNb = 0;
	intensity = 0;
	
	// do other stuff
	
	// tmp
	// lines.clear(); // also properly delete each line ?
}

/*bool lineEffect::grabSomeShapes(){
	// clear
	shapes.clear();
	shapes.resize(0);
	
	// we just want all vertex shapes
	//shapes = shapesHandler::getInstance().getShapesByType("vertexShape");
	
	// then sort them by group
	//map<int, int> groups; // <index, count>
	//groups.clear();
	shapeGroups.clear();
	for(int i=0; i<shapes.size(); i++){
		int group = shapes[i]->getGroupID();
		
		// create map entry ?
		if( shapeGroups.find(group) == shapeGroups.end() ){
			shapeGroups[group] = vector<int>();
		}

		shapeGroups[group].push_back(i);
	}
	
	
}*/

// returns a line from 1 shape vertex to another
// note: when you call this function, mutex must be locked
lineEffectLine lineEffect::getRandomLine( const bool onSameShape){
	
	basicShape* fromShape = shapes[ round( ofRandom(-0.49f, shapes.size()-0.51f) )];
	basicShape* toShape;
	if( onSameShape ) toShape = fromShape;
	else toShape = shapes[ round( ofRandom(-0.49f, shapes.size()-0.51f) )];
	
	return getRandomLine(fromShape, toShape);
	
}

// returns a line from 1 shape vertex to another
// note: when you call this function, mutex must be locked
lineEffectLine lineEffect::getRandomLine(basicShape *_sh1, basicShape *_sh2) {
	
	ofVec2f* from;
	ofVec2f* to;
	if( !_sh1->isReady() || !_sh2->isReady() ){
		from = &basicShape::zeroPoint;
		to = &basicShape::zeroPoint;
	}
	else if( _sh1->isType("vertexShape") ){
		from = ((vertexShape*) _sh1)->getRandomVertexPtr();
		to = ((vertexShape*) _sh2)->getRandomVertexPtr();
		while( _sh1!=_sh2 && from==to ){
			to = ((vertexShape*) _sh2)->getRandomVertexPtr();
		}
	}
	else {
		from = _sh1->getPositionPtr();
		to = _sh2->getPositionPtr();
	}
	
	return lineEffectLine( from, to );
}

void lineEffect::floatListener(durationFloatEventArgs &_args){
	if( _args.track.compare("lineEffectIntensity")!=0 ) return;
	
	ofScopedLock lock(effectMutex);
	intensity = _args.value;
	//lines.push_back();
}

void lineEffect::noteEventListener(ofx::AbletonLiveSet::LSNoteEvent &noteEvent){
	ofScopedLock lock(effectMutex);
	//lines.push_back();
}

void lineEffect::tempoEventListener(mirTempoEventArgs &_args){
	if( _args.isTempoBis ){
		
		ofScopedLock lock(effectMutex);
		if(fromShape==NULL || toShape==NULL) return;
		
		
		int amount = round(mirReceiver::mirCache.zcr*120.0f);
		//if( totalLinesNb%150 == 0){ // get new shapes every X lines
		//cout << amount << endl;
		if(amount > 22){
			fromShape = toShape;
			toShape = shapes[ round( ofRandom(-0.49f, -0.51f+shapes.size()) )];
			cout << "tempoChange" << endl;
		}
		for(int i=0; i<amount; i++){
			lines.push_back( getRandomLine(fromShape, toShape) );
			totalLinesNb++;
		}
	}
}

void lineEffect::onSetEventListener(mirOnSetEventArgs &_args){
	ofScopedLock lock(effectMutex);
	
	if(shapes.size()<=0) return;
	
	if(_args.source.compare("aubioOnSet")==0){
		tempoCalls++;
		for(int i=0; i<1; i++){
			lines.push_back( getRandomLine(fromShape, toShape) );
		}
		
		if(tempoCalls%10==0){
			fromShape = toShape;
			toShape = shapes[ round( ofRandom(-0.49f, -0.51f+shapes.size()) )];
			cout << "onSetChange" << endl;
		}
	}
	
	//else if(_args.source.compare("")==0){}
}

