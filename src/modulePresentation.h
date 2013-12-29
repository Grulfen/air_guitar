#include <Windows.h>
#include <NuiApi.h>
#include "FMOD.hpp"
#include <vector>
#include "cinder/gl/Texture.h"
#include "moduleCapture.h"

using namespace std;

class modulePresentation{

	public:
		float globalVolume;
		modulePresentation();
		void setSounds(int instrument);
		void playNote(int note, float volume);

		/* drawGuitar
		* Pinta una imagen de una guitarra o bajo sobre el imagen del usuario
		* parametros:
		*	point HipPos - posicion de la cadera del usuasio, en espacio de la ventana (pixels)
		*   point chordHandPos - posicion de la mano de acordes, en espacio de la ventana (pixels)
		*	float hipZ - distancia de la camara a la cadera del usuario, espacio del esqueleto (metros)
		*/
		void drawGuitar(point HipPos, point chordHandPos, float hipZ);

		/* setInstrument
		* decide el imagen del instrumento que se esta tocando ahora
		* parametros:
		*	ci::gl::Texture instrument_texture - la textura del nuevo instrumento
		*/
		void setInstrument(ci::gl::Texture instrument_texture);
	
	private:
		FMOD::System	*mSystem;
		FMOD::Channel	*mChannel;
		vector<FMOD::Sound*>  mSounds;
		string notes [12];
		ci::gl::Texture instrument_texture;
};