#include <Windows.h>
#include <NuiApi.h>
#include "FMOD.hpp"
#include <vector>
#include "cinder/gl/Texture.h"
#include "moduleCapture.h"

using namespace std;

class modulePresentation{

	public:
		modulePresentation();
		void setSounds(int instrument);
		void playNote(int note, float volume);
		void drawGuitar(ci::gl::Texture texture, point HipPos, point playHandPos);
	
	private:
		FMOD::System	*mSystem;
		FMOD::Channel	*mChannel;
		vector<FMOD::Sound*>  mSounds;
		string notes [12];
		ci::gl::Texture guitar;
};