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
	void	prepareSettings( ci::app::AppBasic::Settings *settings );
	void	setup();
	void	shutdown();
	void	update();

	moduleCapture capture;
	ci::Surface8u background;

private:
	modulePresentation mPresentation;
	moduleProcessing mProcessing;
	HandsHip *handsHip;
	ci::gl::TextureFontRef mFont;
	std::string mText;
};

// Imports
using namespace ci;
using namespace ci::app;
using namespace std;

// Render
void AirGuitarApp::draw()
{
	ci::gl::clear();
	mFont->drawString(mText, cinder::Rectf(0,0,800,200));
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
		if(mText == "hejsan"){
			mText = "apfan";
		} else {
			mText = "hejsan";
		}
	}
}

// Prepare window
void AirGuitarApp::prepareSettings( Settings *settings )
{
        settings->setWindowSize( 800, 600 );
        settings->setFrameRate( 60.0f );
}

// Set up
void AirGuitarApp::setup(){
	mFont = ci::gl::TextureFont::create(Font("Arial Black", 40));
	mText = "durr";
	capture = moduleCapture();
	background = ci::Surface8u(capture.resolution.width, capture.resolution.height, false);
	mPresentation.setSounds(0);
}

// Called on exit
void AirGuitarApp::shutdown()
{
}

// Runs update logic
void AirGuitarApp::update()
{
	handsHip = capture.formHandsHip(0);
	if(handsHip != NULL){
		mText = std::to_string(handsHip->chordHandPosition.x) + ", " + std::to_string(handsHip->chordHandPosition.y) + ", " + std::to_string(handsHip->chordHandPosition.z);
		mText += "\n";
		mText += std::to_string(handsHip->playingHandPosition.x) + ", " + std::to_string(handsHip->playingHandPosition.y) + ", " + std::to_string(handsHip->playingHandPosition.z);
		mText += "\n";
		mText += std::to_string(handsHip->hipPosition.x) + ", " + std::to_string(handsHip->hipPosition.y) + ", " + std::to_string(handsHip->hipPosition.z);
		mText += "\n";
		mProcessing.calculateVolume(handsHip);
		mProcessing.calculateTone(handsHip);
		if(mProcessing.playedNote(handsHip)){
			mPresentation.playNote(mProcessing.tone,mProcessing.volume);
		}
		mText += std::to_string(mProcessing.tone);
	} else {
		mText = "Skeleton not found";
	}
}

// Run application
CINDER_APP_BASIC( AirGuitarApp, RendererGl )