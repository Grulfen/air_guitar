#include <vector>

using namespace std;

struct HandsHip{
	vector<float> chordHandPosition;
	vector<float> hipPosition;
	vector<float> playingHandPosition;
};

class moduleProcessing{
	
	moduleProcessing();

	public: 
		void calculateTone(HandsHip *handship);
		void calculateVolume(HandsHip *handship);
		bool playedNote(HandsHip *handship);

		float volume;
		int tone;

	private:
		float E, F, Fsharp, G, Gsharp, A, Asharp, B, C, Csharp, D, Dsharp;

		float boxSize;

		float maxDistance;

		vector<float> lastPlayingHand;

		bool isOnPlayingArea,notePlayed;
					
};