#include <vector>
#include "moduleCapture.h"

using namespace std;

class moduleProcessing{
	
	

	public: 
		moduleProcessing();
		void calculateTone(HandsHip *handship);
		void calculateVolume(HandsHip *handship);
		bool playedNote(HandsHip *handship);
		float distance(HandsHip *handship);

		float volume;
		int tone;

	private:
		float E, F, Fsharp, G, Gsharp, A, Asharp, B, C, Csharp, D, Dsharp;

		float boxSize;

		float maxDistance;

		Vector4 lastPlayingHand;

		bool isOnPlayingArea,notePlayed;
					
};