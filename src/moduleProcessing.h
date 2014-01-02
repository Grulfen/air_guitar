#include <vector>
#include "moduleCapture.h"

using namespace std;

class moduleProcessing{
	
	

	public: 
		moduleProcessing();
		void calculateTone(HandsHip *handship); //Método para calcular el tono
		void calculateVolume(HandsHip *handship); //Método para calcular el volumen
		bool playedNote(HandsHip *handship); //Método para ver si una nota fue tocada.

		//Las variables del tono y volumen
		float volume;
		int tone;

	private:
		float E, F, Fsharp, G, Gsharp, A, Asharp, B, C, Csharp, D, Dsharp; //Las distancias entre en metros entre la cadera y la mano que toca los acordes para cada nota.

		float boxSize; //El tamaño del area de reproducción por donde pasara la mano de reproducción.

		float maxDistance; //La distancia máxima de la mano de reproducción entre dos frames usado para normalizar el volumen que calculamos en base a la velocidad.

		Vector4 lastPlayingHand; //La estructura de la mano de reproducción del frame anterior.

		bool isOnPlayingArea,notePlayed; //Variables que indican si la mano de reproducción se encuentra sobre el área de reproducción y si una nota ya fue tocada.
					
};