// Includes
#include <Windows.h>
#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/ImageIo.h"
#include "cinder/params/Params.h"
#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
#include "NuiApi.h"
#include "moduleCapture.h"
#include "moduleProcessing.h"
#include "modulePresentation.h"
#include "ciUI.h" 
/* 
* This application demonstrates how to acquire and display 
* skeleton data.
*/ 
class AirGuitarApp : public ci::app::AppBasic 
{

public:

	// Cinder callbacks
	void	draw();
	void	keyDown( ci::app::KeyEvent event );
//	void	prepareSettings( ci::app::AppBasic::Settings *settings );
	void	setup();
	void	shutdown();
	void	update();
	void	guiEvent(ciUIEvent *event);

	moduleCapture capture;

private:
	modulePresentation mPresentation;
	moduleProcessing mProcessing;
	HandsHip *handsHip;
	float guiWidth; 
    float guiHeight; 
    float dim; 
    float length; 
	ci::gl::Texture *backgroundTex;
	ci::gl::Texture guitar;
	std::string mText;
	point chordHandPos;
	point playHandPos;
	point HipPos;    
    ColorA backgroundColor; 
    ciUICanvas *gui; 
	int instrument;
	bool rightHanded;
};

// Imports
using namespace ci;
using namespace ci::app;
using namespace std;

// Render
void AirGuitarApp::draw()
{
	ci::gl::clear(backgroundColor);
	
	if(backgroundTex != NULL){
		gl::draw(*backgroundTex, getWindowBounds());
	}
	if(handsHip != NULL){
		mPresentation.drawGuitar(guitar, HipPos, chordHandPos);
		ci::gl::drawSolidCircle( Vec2f( chordHandPos.x, chordHandPos.y), 10);
		ci::gl::drawSolidCircle( Vec2f( playHandPos.x, playHandPos.y), 10);
		ci::gl::drawSolidCircle( Vec2f( HipPos.x, HipPos.y), 10);
	}
	 gl::enableAlphaBlending();
	 gui->draw();
	 gl::disableAlphaBlending();
//	mFont->drawString(mText, cinder::Rectf(0,0,800,200));
}

// Handles key press
void AirGuitarApp::keyDown( KeyEvent event )
{

	// Quit, toggle fullscreen
	switch ( event.getCode() ) {
	case KeyEvent::KEY_q:
		quit();
		break;
	case KeyEvent::KEY_f:
		setFullScreen( !isFullScreen() );
		break;
	case KeyEvent::KEY_SPACE:
		/*if(mText == "hejsan"){
			mText = "apfan";
		} else {
			mText = "hejsan";
		}*/
		break;
	}
}

// Prepare window
/*void AirGuitarApp::prepareSettings( Settings *settings )
{
        settings->setWindowSize( 800, 600 );
        settings->setFrameRate( 60.0f );
}*/

// Set up
void AirGuitarApp::setup(){
	backgroundTex = NULL;
     
    backgroundColor = ColorA(233.0/255.0, 52.0/255.0, 27.0/255.0);     
    guiWidth = getWindowWidth()*.4; 
    guiHeight= getWindowHeight()*.75; 
    dim = guiWidth*.0625; 
    length = guiWidth-CI_UI_GLOBAL_WIDGET_SPACING*2;

	rightHanded = true;

	vector<string> instruments;
	instruments.push_back("Electric Guitar");
	instruments.push_back("Bass");


	vector<string> handedness;
	handedness.push_back("Right");
	handedness.push_back("Left");
	
    
    gui = new ciUICanvas(10,10,guiWidth, guiHeight);
	gui->addWidgetDown(new ciUILabel(0,25, "AIRGUITAR", CI_UI_FONT_MEDIUM)); 
	gui->addWidgetDown(new ciUISlider(length - CI_UI_GLOBAL_WIDGET_SPACING, dim, 0.0, 1.0, 1.0, "Volume")); 
	gui->addWidgetDown(new ciUIRadio(20.0,20.0,"Instrument",instruments,0));
	((ciUIRadio *) gui->getWidget("Instrument"))->activateToggle("Electric Guitar");
	gui->addWidgetDown(new ciUIRadio(20.0,20.0,"Handedness",handedness,0));
	((ciUIRadio *) gui->getWidget("Handedness"))->activateToggle("Right");
   
    gui->registerUIEvents(this, &AirGuitarApp::guiEvent);

	capture = moduleCapture();
	mPresentation.setSounds(0);
	guitar = loadImage(getAssetPath("electric/image/electric_right.png"));
	OutputDebugStringW(L"In setup end\n");
}

void AirGuitarApp::guiEvent(ciUIEvent *event){
	string name = event->widget->getName();

	if(name == "Volume")
	{
		ciUISlider *slider = (ciUISlider *) event->widget; 
		mPresentation.globalVolume = slider->getScaledValue(); 
	}
	if(name == "Electric Guitar")
	{	
		mPresentation.setSounds(0);
	}
	if(name == "Bass")
	{	
		mPresentation.setSounds(1);
	}
	if(name == "Right")
	{
		rightHanded = true;
	}
	if(name == "Left")
	{
		rightHanded = false;
	}
}

// Called on exit
void AirGuitarApp::shutdown()
{
}

// Runs update logic
void AirGuitarApp::update()
{
	
	gui->update(); 
	handsHip = capture.formHandsHip(0,rightHanded);
	if(handsHip != NULL){
		/*mText = std::to_string(handsHip->chordHandPosition.x) + ", " + std::to_string(handsHip->chordHandPosition.y) + ", " + std::to_string(handsHip->chordHandPosition.z);
		mText += "\n";
		mText += std::to_string(handsHip->playingHandPosition.x) + ", " + std::to_string(handsHip->playingHandPosition.y) + ", " + std::to_string(handsHip->playingHandPosition.z);
		mText += "\n";
		mText += std::to_string(handsHip->hipPosition.x) + ", " + std::to_string(handsHip->hipPosition.y) + ", " + std::to_string(handsHip->hipPosition.z);
		mText += "\n";*/
		mProcessing.calculateVolume(handsHip);
		mProcessing.calculateTone(handsHip);
		if(mProcessing.playedNote(handsHip)){
			mPresentation.playNote(mProcessing.tone,mProcessing.volume);
		}
		//mText += std::to_string(mProcessing.tone);
	} else {
		//mText = "Skeleton not found";
	}
	ci::gl::Texture *tmp = capture.getNextFrame();
	if (tmp != NULL){
		backgroundTex = tmp;
	}
}

// Run application
CINDER_APP_BASIC( AirGuitarApp, RendererGl )