#include "moduleProcessing.h"
#include <vector>

using namespace std;

moduleProcessing::moduleProcessing(){
			E = 120.0;
			F = 110.0;
			Fsharp = 100.0;
			G = 90.0;
			Gsharp = 80.0;
			A = 70.0;
			Asharp = 60.0;
			B = 50.0;
			C = 40.0;
			Csharp = 30.0;
			D = 20.0;
			Dsharp = 10.0;

			boxSize = 20.0;
			maxDistance = 5.0;
}


void moduleProcessing::calculateTone(HandsHip *handship){
	vector<float> chordHand = handship->chordHandPosition;
	vector<float> hip = handship->hipPosition;

	float distance = (chordHand[0]-hip[0])*(chordHand[0]-hip[0]) + (chordHand[1]-hip[1])*(chordHand[1]-hip[1]);

	if(distance>E){
		this->tone = 0;
	}else if(distance>F){
		this->tone = 1;
	}else if(distance>Fsharp){
		this->tone = 2;
	}else if(distance>G){
		this->tone = 3;
	}else if(distance>Gsharp){
		this->tone = 4;
	}else if(distance>A){
		this->tone = 5;
	}else if(distance>Asharp){
		this->tone = 6;
	}else if(distance>B){
		this->tone = 7;
	}else if(distance>C){
		this->tone = 8;
	}else if(distance>Csharp){
		this->tone = 9;
	}else if(distance>D){
		this->tone = 10;
	}else if(distance>Dsharp){
		this->tone = 11;
	}
}

void moduleProcessing::calculateVolume(HandsHip *handship){
	vector<float> playingHand = handship->playingHandPosition;

	float distance = (playingHand[0]-lastPlayingHand[0])*(playingHand[0]-lastPlayingHand[0]) + (playingHand[1]-lastPlayingHand[1])*(playingHand[1]-lastPlayingHand[1]);

	if(distance>maxDistance){
		this->volume = 1.0;
	}else{
		this->volume = distance/maxDistance;
	}
	lastPlayingHand = playingHand;
}

bool moduleProcessing::playedNote(HandsHip *handship){
	vector<float> hip = handship->hipPosition;
	vector<float> playingHand = handship->playingHandPosition;

	float ax = hip[0]-boxSize,
		ay = hip[1]-boxSize,
		bx = hip[0]+boxSize,
		by = hip[1]-boxSize,
		dx = hip[0]+boxSize,
		dy = hip[1]+boxSize;

	float bax = bx - ax,
		bay = by - ay,
		dax = dx - ax,
		day = dy -ay;

	if ((playingHand[0] - ax) * bax + (playingHand[1] - ay) * bay < 0.0) this->isOnPlayingArea = false;
	if ((playingHand[0] - bx) * bax + (playingHand[1] - by) * bay > 0.0) this->isOnPlayingArea = false;
	if ((playingHand[0] - ax) * dax + (playingHand[1] - ay) * day < 0.0) this->isOnPlayingArea = false;
	if ((playingHand[0] - dx) * dax + (playingHand[1] - dy) * day > 0.0) this->isOnPlayingArea = false;

	if(isOnPlayingArea && !notePlayed){
		notePlayed = true;
		return true;
	}
	
	if(!isOnPlayingArea) notePlayed = false;

	return false;
}