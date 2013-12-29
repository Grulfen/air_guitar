#ifndef MODULECAPTURE_H
#define MODULECAPTURE_H

#include <Windows.h>
#include <NuiApi.h>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include <iostream>
#include <vector>


/* estructura que contiene la resuloción de la ventana de la aplicacion
*/
struct resolution_t{
	int width;
	int height;
};

/* estructura para representar un punto
*/
struct point{
	float x;
	float y;
};

/* Estructura con los datos interesantes del esqueleto
*/
struct HandsHip{
	Vector4 hipPosition;
	Vector4 playingHandPosition;
	Vector4 chordHandPosition;
};


/* moduleCapture
/  Modulo con la interfaz al Kinect
*/
class moduleCapture
{
public:
	moduleCapture();
	~moduleCapture();
	resolution_t resolution;

	/* formHandsHip
	* parametros:
	*	bool righthanded - indica si el usuario toca con la mano derecha o izquirda, indicado por la intefaz del usuario
	* crea una estructura handsHip con los datos interesantes del esqueleto y retorna un puntero a la 
	* estructura, si había un error retorna NULL
	*/
	HandsHip *formHandsHip(bool rightHanded=true);

	/* getNextFrame
	* returna una textura con los canales del color BGRA de la camera del kinect
	*/
	ci::gl::Texture *getNextFrame();

	// Puntero al objeto que representa el kinect
	INuiSensor *sensor;

	/* SkeletonPointToScreen
	* Transforma un punto en el espacio del esqueleto a un punto en el espacio de la pantalla (en pixeles)
	* parametros:
	*	Vector4 skeletonPoint - un punto del esqueleto
	*   int height - altura en pixeles de la ventana
	*	int width - ancho en pixeles de la ventana
	*/
	point SkeletonPointToScreen(Vector4 skeletonPoint, int height, int width);
private:

	HANDLE colorStreamHandle;

	NUI_IMAGE_FRAME *pColorImageFrame;

	NUI_SKELETON_FRAME *pSkeletonFrame;

	NUI_SKELETON_DATA *skeleton;

	/* getSkeleton
	 * retorna un puntero al NUI_SKELETON_DATA con información sobre los esqueletos que detecta el Kinect
	 * Usado por formHandsHip
	 */
	NUI_SKELETON_DATA *getSkeleton();
};

#endif