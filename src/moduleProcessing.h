#include <vector>
#include "moduleCapture.h"

using namespace std;

class moduleProcessing{
	
	

	public: 
		moduleProcessing();
		void calculateTone(HandsHip *handship); //M�todo para calcular el tono
		void calculateVolume(HandsHip *handship); //M�todo para calcular el volumen
		bool playedNote(HandsHip *handship); //M�todo para ver si una nota fue tocada.

		//Las variables del tono y volumen
		float volume;
		int tone;

	private:
		float E, F, Fsharp, G, Gsharp, A, Asharp, B, C, Csharp, D, Dsharp; //Las distancias entre en metros entre la cadera y la mano que toca los acordes para cada nota.

		float boxSize; //El tama�o del area de reproducci�n por donde pasara la mano de reproducci�n.

		float maxDistance; //La distancia m�xima de la mano de reproducci�n entre dos frames usado para normalizar el volumen que calculamos en base a la velocidad.

		Vector4 lastPlayingHand; //La estructura de la mano de reproducci�n del frame anterior.

		bool isOnPlayingArea,notePlayed; //Variables que indican si la mano de reproducci�n se encuentra sobre el �rea de reproducci�n y si una nota ya fue tocada.
					
};