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

private:
	modulePresentation mPresentation;
	moduleProcessing mProcessing;
	HandsHip *handsHip;

	ci::gl::TextureFontRef mFont;
	ci::gl::Texture *backgroundTex;
	ci::gl::Texture guitar;
	std::string mText;
	point chordHandPos;
	point playHandPos;
	point HipPos;
};

// Imports
using namespace ci;
using namespace ci::app;
using namespace std;

// Render
void AirGuitarApp::draw()
{
	ci::gl::clear();
	if(backgroundTex != NULL){
		gl::draw(*backgroundTex, getWindowBounds());
	}
	mFont->drawString(mText, cinder::Rectf(0,0,800,200));
	if(handsHip != NULL){
		mPresentation.drawGuitar(guitar, HipPos, chordHandPos);
		ci::gl::drawSolidCircle( Vec2f( chordHandPos.x, chordHandPos.y), 10);
		ci::gl::drawSolidCircle( Vec2f( playHandPos.x, playHandPos.y), 10);
		ci::gl::drawSolidCircle( Vec2f( HipPos.x, HipPos.y), 10);
	}
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
	backgroundTex = NULL;
	mPresentation.setSounds(0);
	guitar = loadImage(getAssetPath("electric/image/electric_right.png"));
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
		chordHandPos = capture.SkeletonPointToScreen(handsHip->chordHandPosition, ci::app::getWindowHeight(), ci::app::getWindowWidth());
		playHandPos = capture.SkeletonPointToScreen(handsHip->playingHandPosition, ci::app::getWindowHeight(), ci::app::getWindowWidth());
		HipPos = capture.SkeletonPointToScreen(handsHip->hipPosition, ci::app::getWindowHeight(), ci::app::getWindowWidth());
		mText = std::to_string(mProcessing.tone);
	} else {
		mText = "Skeleton not found";
	}
	ci::gl::Texture *tmp = capture.getNextFrame();
	if (tmp != NULL){
		backgroundTex = tmp;
	}
}

// Run application
CINDER_APP_BASIC( AirGuitarApp, RendererGl )