#include "testApp.h"
#include <vector>
#include <Windows.h>
#include <NuiApi.h>

//--------------------------------------------------------------
void testApp::setup(){

	std::vector<std::string> guitars;
	guitars.push_back("Acustic");
	guitars.push_back("Electric");

	std::vector<std::string> handedness;
	handedness.push_back("Left");
	handedness.push_back("Right");

	gui = new ofxUICanvas();
	gui->addLabel("Air Guitar", OFX_UI_FONT_LARGE);
    gui->addSpacer(0, 15);
	gui->setColorBack(ofxUIColor(40, 100, 60, 150));
	gui->addIntSlider("Volume", 0, 100, 25);
	gui->addSpacer();
	gui->addLabel("Guitars");
	gui->addRadio("RADIO",  guitars);
	gui->addLabel("Handedness");
	gui->addRadio("HANDS", handedness);
	background = new ofImage("Img/background.jpg");
}
//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	background->draw(0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::exit(){
	gui->saveSettings("GUI/guiSettings.xml");
	delete gui;
}

void testApp::guiEvent(ofxUIEventArgs &e){
	if(e.getName() == "Volume"){
		ofxUIIntSlider *slider = (ofxUIIntSlider *) e.widget;
		volume = slider->getValue();
		std::cout << volume << std::endl;
	}
}
