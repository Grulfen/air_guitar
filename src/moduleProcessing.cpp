#include "moduleProcessing.h"
#include <vector>

using namespace std;

moduleProcessing::moduleProcessing(){

			//Las distancias entre en metros entre la cadera y la mano que toca los acordes para cada nota.
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

			boxSize = 0.08; //El tamaño del area de reproducción por donde pasara la mano de reproducción.
			maxDistance = 0.01; //La distancia máxima de la mano de reproducción entre dos frames usado para normalizar el volumen que calculamos en base a la velocidad.
}


void moduleProcessing::calculateTone(HandsHip *handship){
	Vector4 chordHand = handship->chordHandPosition;
	Vector4 hip = handship->hipPosition;

	//La distancia entre la mano de los acordes y la cadera.
	float distance = (chordHand.x-hip.x)*(chordHand.x-hip.x) + (chordHand.y-hip.y)*(chordHand.y-hip.y); 

	//Asignamos el tono correspondiente con la distancia
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

	//Distancia entre la mano de reproducción actual y la mano de reproducción del frame anterior.
	float distance = (playingHand.x-lastPlayingHand.x)*(playingHand.x-lastPlayingHand.x) + (playingHand.y-lastPlayingHand.y)*(playingHand.y-lastPlayingHand.y);

	//Si la distancia es mayor a la distancia máxima establecida el volumen sera 1.0 de lo contrario sera un valor entre 0.0 y 1.0
	if(distance>maxDistance){
		this->volume = 1.0;
	}else{
		this->volume = distance/maxDistance;
	}

	lastPlayingHand = playingHand; //Asignamos la mano de reproducción actual como la mano de reproducción anterior
}


bool moduleProcessing::playedNote(HandsHip *handship){
	Vector4 hip = handship->hipPosition;
	Vector4 playingHand = handship->playingHandPosition;

	//Calculamos los puntos superior izquierda, superior derecha e inferior derecha del area de reproducción pasado en la posición de la cadera
	//y el tamaño del area de reproducción predefinida.
	float ax = hip.x-boxSize,
		ay = hip.y-boxSize,
		bx = hip.x+boxSize,
		by = hip.y-boxSize,
		dx = hip.x+boxSize,
		dy = hip.y+boxSize;

	//Diferencias entre los puntos
	float bax = bx - ax,
		bay = by - ay,
		dax = dx - ax,
		day = dy -ay;

	
	isOnPlayingArea = true;

	//Si conectamos el punto a los tres vértices del rectángulo entonces los ángulos entre los segmentos y los lados deben ser agudos
	//Si no son agudos se indica que la mano de reproduccion no esta sobre el área de reproducción.
	if ((playingHand.x - ax) * bax + (playingHand.y - ay) * bay < 0.0) this->isOnPlayingArea = false;
	if ((playingHand.x - bx) * bax + (playingHand.y - by) * bay > 0.0) this->isOnPlayingArea = false;
	if ((playingHand.x - ax) * dax + (playingHand.y - ay) * day < 0.0) this->isOnPlayingArea = false;
	if ((playingHand.x - dx) * dax + (playingHand.y - dy) * day > 0.0) this->isOnPlayingArea = false;

	//Si esta sobre el área de reproducción pero no ha reproducido una nota devolver como true para que se reproduzca la nota.
	//Esto se hace para que no se reproduzca el sonido varias veces mientras se encuentre la mano sobre el área de reproducción.
	if(isOnPlayingArea && !notePlayed){
		notePlayed = true;
		return true;
	}
	
	//Si la mano salio sobre el área de reproducción, se marca como falso la variable notePlayed para que pueda volver a tocar una nota.
	if(!isOnPlayingArea) notePlayed = false;

	return false;
}