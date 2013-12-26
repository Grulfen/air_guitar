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
		void drawGuitar(point HipPos, point chordHandPos);
		void setInstrument(ci::gl::Texture instrument_texture);
	
	private:
		FMOD::System	*mSystem;
		FMOD::Channel	*mChannel;
		vector<FMOD::Sound*>  mSounds;
		string notes [12];
		ci::gl::Texture instrument_texture;
};