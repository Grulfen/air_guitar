#include "moduleProcessing.h"
#include <vector>

using namespace std;

moduleProcessing::moduleProcessing(){
			E = 0.49;
			F = 0.45;
			Fsharp = 0.41;
			G = 0.37;
			Gsharp = 0.33;
			A = 0.29;
			Asharp = 0.25;
			B = 0.21;
			C = 0.17;
			Csharp = 0.13;
			D = 0.09;
			Dsharp = 0.05;

			boxSize = 0.08;
			maxDistance = 0.01;
}


void moduleProcessing::calculateTone(HandsHip *handship){
	Vector4 chordHand = handship->chordHandPosition;
	Vector4 hip = handship->hipPosition;

	float distance = (chordHand.x-hip.x)*(chordHand.x-hip.x) + (chordHand.y-hip.y)*(chordHand.y-hip.y);

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
	Vector4 playingHand = handship->playingHandPosition;

	float distance = (playingHand.x-lastPlayingHand.x)*(playingHand.x-lastPlayingHand.x) + (playingHand.y-lastPlayingHand.y)*(playingHand.y-lastPlayingHand.y);

	if(distance>maxDistance){
		this->volume = 1.0;
	}else{
		this->volume = distance/maxDistance;
	}
	lastPlayingHand = playingHand;
}

float moduleProcessing::distance(HandsHip *handship){
	Vector4 chordHand = handship->chordHandPosition;
	Vector4 hip = handship->hipPosition;

	float distance = (chordHand.x-hip.x)*(chordHand.x-hip.x) + (chordHand.y-hip.y)*(chordHand.y-hip.y);

	return distance;
}

bool moduleProcessing::playedNote(HandsHip *handship){
	Vector4 hip = handship->hipPosition;
	Vector4 playingHand = handship->playingHandPosition;

	float ax = hip.x-boxSize,
		ay = hip.y-boxSize,
		bx = hip.x+boxSize,
		by = hip.y-boxSize,
		dx = hip.x+boxSize,
		dy = hip.y+boxSize;

	float bax = bx - ax,
		bay = by - ay,
		dax = dx - ax,
		day = dy -ay;


	isOnPlayingArea = true;

	if ((playingHand.x - ax) * bax + (playingHand.y - ay) * bay < 0.0) this->isOnPlayingArea = false;
	if ((playingHand.x - bx) * bax + (playingHand.y - by) * bay > 0.0) this->isOnPlayingArea = false;
	if ((playingHand.x - ax) * dax + (playingHand.y - ay) * day < 0.0) this->isOnPlayingArea = false;
	if ((playingHand.x - dx) * dax + (playingHand.y - dy) * day > 0.0) this->isOnPlayingArea = false;


	if(isOnPlayingArea && !notePlayed){
		notePlayed = true;
		return true;
	}
	
	if(!isOnPlayingArea) notePlayed = false;

	return false;
}