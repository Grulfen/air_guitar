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
 * Aplicación principal de air guitar!
 */
class AirGuitarApp : public ci::app::AppBasic 
{

public:

	// Cinder callbacks
	void	draw();
	void	keyDown( ci::app::KeyEvent event );
	void	setup();
	void	shutdown();
	void	update();
	void	guiEvent(ciUIEvent *event);

private:
	moduleCapture capture;
	modulePresentation mPresentation;
	moduleProcessing mProcessing;
	HandsHip *handsHip;
	float guiWidth; 
    float guiHeight; 
    float dim; 
    float length; 
	ci::gl::Texture *backgroundTex;
	point chordHandPos;
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
	// Limpia la pantalla
	ci::gl::clear(backgroundColor);
	
	// Pinta el fondo (imagens del usario)
	if(backgroundTex != NULL){
		gl::draw(*backgroundTex, getWindowBounds());
	}

	// Pinta el UI (con canal alpha)
	gl::enableAlphaBlending();
	gui->draw();
	gl::disableAlphaBlending();

	// Pintar guitarra
	if(handsHip != NULL){
		mPresentation.drawGuitar(HipPos, chordHandPos, handsHip->hipPosition.z);
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

// Set up
void AirGuitarApp::setup(){
	backgroundTex = NULL;
    backgroundColor = ColorA(233.0/255.0, 52.0/255.0, 27.0/255.0);   

	// Initializa el GUI
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

	//Initializa módulos
	capture = moduleCapture();
	mPresentation.setSounds(0);
	mPresentation.setInstrument(loadImage(getAssetPath("electric/image/electric_right.png")));
	OutputDebugStringW(L"In setup end\n");
}

void AirGuitarApp::guiEvent(ciUIEvent *event){
	string name = event->widget->getName();

	if(name == "Volume")
	{
		ciUISlider *slider = (ciUISlider *) event->widget; 
		mPresentation.globalVolume = slider->getScaledValue(); 
	}

	// Cambia sonidos y imagen según el instrumento indicado por el UI
	if(name == "Electric Guitar")
	{	
		mPresentation.setSounds(0);
		mPresentation.setInstrument(loadImage(getAssetPath("electric/image/electric_right.png")));
	}
	if(name == "Bass")
	{	
		mPresentation.setSounds(1);
		mPresentation.setInstrument(loadImage(getAssetPath("bass/image/bass.png")));
	}

	// Cambia la lateralidad
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
	// Actualiza el gui
	gui->update(); 

	// Actualiza las posiciones de la cadera y las manos
	handsHip = capture.formHandsHip(rightHanded);
	if(handsHip != NULL){
		// Calcula las nuevas posiciones en pixeles
		chordHandPos = capture.SkeletonPointToScreen(handsHip->chordHandPosition, ci::app::getWindowHeight(), ci::app::getWindowWidth());
        HipPos = capture.SkeletonPointToScreen(handsHip->hipPosition, ci::app::getWindowHeight(), ci::app::getWindowWidth());

		// Calcula volumen y tono
		mProcessing.calculateVolume(handsHip);
		mProcessing.calculateTone(handsHip);

		// Toca tono si la mano esta sobre las cuerdas
		if(mProcessing.playedNote(handsHip)){
			mPresentation.playNote(mProcessing.tone, mProcessing.volume);
		}
	}

	// Actualiza la imagen del usuario
	ci::gl::Texture *tmp = capture.getNextFrame();
	if (tmp != NULL){
		backgroundTex = tmp;
	}
}

// Run application
CINDER_APP_BASIC( AirGuitarApp, RendererGl )