#include "modulePresentation.h"
#include "cinder/ImageIo.h"
#include "cinder/app/AppBasic.h"
#include <vector>




using namespace cinder;
using namespace ci::app;
using namespace std;

modulePresentation::modulePresentation(){
	notes[0] = "E1";
	notes[1] = "F1";
	notes[2] = "F#1";
	notes[3] = "G1";
	notes[4] = "G#1";
	notes[5] = "A1";
	notes[6] = "A#1";
	notes[7] = "B1";
	notes[8] = "C2";
	notes[9] = "C#2";
	notes[10] = "D2";
	notes[11] = "D#2";
	FMOD::System_Create( &mSystem );
	globalVolume = 1.0;
    mSystem->init( 32, FMOD_INIT_NORMAL | FMOD_INIT_ENABLE_PROFILE, NULL );
}

void modulePresentation::drawGuitar(point HipPos, point chordHandPos, float hipZ)
{
	// Calcula el valor del escalamiento de la guitarra como funcion de la distanca de cadera a la camara 
	float scaleValueW = ci::app::getWindowWidth()/(16.0*30.0)/(hipZ);
	float scaleValueH = ci::app::getWindowHeight()/(9.0*30.0)/(hipZ);

	// Habilita canal alpha
	gl::enableAlphaBlending();

	// Nuevo visto del modelo
	gl::pushModelView();

	// Las transformaciones en orden reverso
	// Pone guitarra sobra la cadera del usuario
	gl::translate(HipPos.x, HipPos.y);

	// Rota la guitarra asi que esta entra la cadera y la mano de acordes
	gl::rotate(180 + 180/3.14*atan2(HipPos.y - chordHandPos.y, HipPos.x - chordHandPos.x));

	// Escala la guitarra
	gl::scale(scaleValueW, scaleValueH);

	// Mueve la guitarra asi que el centro esta en (0, 0)
	gl::translate(-120, -70);

	// Pinta la guitarra
	gl::draw(instrument_texture);

	// Listo con las transformaciones
	gl::popModelView();

	// Deshabilita canal alpha
	gl::disableAlphaBlending();
}

void modulePresentation::setInstrument(ci::gl::Texture instrument_texture)
{
	this->instrument_texture = instrument_texture;
}

void modulePresentation::setSounds(int inst){
	string instrument;
	switch(inst){
		case 0:
			instrument = "electric";
			break;
		case 1:
			instrument = "bass";
			break;
	}
	mSounds.clear();
	for(int i=0;i<12;i++){
		FMOD::Sound *mSound;
		mSystem->createSound( getAssetPath( instrument + "/mp3/" + notes[i]+ ".mp3").string().c_str(), FMOD_SOFTWARE, NULL, &mSound );
		mSounds.push_back(mSound);
	}
}

void modulePresentation::playNote(int note, float volume){
	mChannel->stop();
	mSystem->playSound( FMOD_CHANNEL_FREE, mSounds[note], false, &mChannel );
	mChannel->setVolume(volume*globalVolume);
}