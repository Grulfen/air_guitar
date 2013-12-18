// Includes
#include <Windows.h>
#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/params/Params.h"
#include "cinder/Utilities.h"
#include "NuiApi.h"
#include "moduleCapture.h"

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
};

// Imports
using namespace ci;
using namespace ci::app;
using namespace std;

// Render
void AirGuitarApp::draw()
{
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
void AirGuitarApp::setup()
{
	capture = moduleCapture();
}

// Called on exit
void AirGuitarApp::shutdown()
{
}

// Runs update logic
void AirGuitarApp::update()
{
}

// Run application
CINDER_APP_BASIC( AirGuitarApp, RendererGl )