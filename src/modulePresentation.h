#include "FMOD.hpp"
#include <vector>

using namespace std;

class modulePresentation{

	public:
		float globalVolume;
		modulePresentation();
		void setSounds(int instrument);
		void playNote(int note, float volume);
	
	private:
		FMOD::System	*mSystem;
		FMOD::Channel	*mChannel;
		vector<FMOD::Sound*>  mSounds;
		string notes [12];
};