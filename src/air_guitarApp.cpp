// Includes
#include <Windows.h>
#include "NuiApi.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/ImageIo.h"
#include "cinder/params/Params.h"
#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
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
	ci::Surface8u *background;

private:
	modulePresentation mPresentation;
	moduleProcessing mProcessing;
	HandsHip *handsHip;

	ci::gl::TextureFontRef mFont;
	ci::gl::Texture backgroundTex;
	std::string mText;
	float chordHandX;
	float chordHandY;
	float playHandX;
	float playHandY;
	float HipY;
	float HipX;
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
	if(handsHip != NULL){

		ci::gl::drawSolidCircle( Vec2f( chordHandX, chordHandY), 10);
		ci::gl::drawSolidCircle( Vec2f( playHandX, playHandY), 10);
		ci::gl::drawSolidCircle( Vec2f( HipX, HipY), 10);
	}
	//if(background != NULL){
		//gl::draw(backgroundTex, getWindowBounds());
	//}
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
	//background = new ci::Surface8u(capture.resolution.width, capture.resolution.height, false);
	mPresentation.setSounds(0);
	OutputDebugStringW(L"In setup end\n");
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
		mProcessing.calculateVolume(handsHip);
		mProcessing.calculateTone(handsHip);
		if(mProcessing.playedNote(handsHip)){
			mPresentation.playNote(mProcessing.tone,mProcessing.volume);
		}
		chordHandX = capture.resolution.width*(2 + handsHip->chordHandPosition.x)/4;
		chordHandY = capture.resolution.width - capture.resolution.width*(2 + handsHip->chordHandPosition.y)/4;
		playHandX = capture.resolution.width*(2 + handsHip->playingHandPosition.x)/4;
		playHandY = capture.resolution.width - capture.resolution.width*(2 + handsHip->playingHandPosition.y)/4;
		HipX = capture.resolution.width*(2 + handsHip->hipPosition.x)/4;
		HipY = capture.resolution.width - capture.resolution.width*(2 + handsHip->hipPosition.y)/4;
		mText = std::to_string(mProcessing.tone);
	} else {
		mText = "Skeleton not found";
	}
	//capture.getNextFrame(background);
	//backgroundTex = ci::gl::Texture(*background);
}

// Run application
CINDER_APP_BASIC( AirGuitarApp, RendererGl )