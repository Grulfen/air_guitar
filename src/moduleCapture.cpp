#include "moduleCapture.h"
#include "cinder/ImageIo.h"
#include "string"

/* moduleCapture
/  Modulo con la interfaz al Kinect
*/
moduleCapture::moduleCapture()
{
	sensor = NULL;
	resolution.height = 480;
	resolution.width = 640;
	pSkeletonFrame = new NUI_SKELETON_FRAME;
	skeleton = new NUI_SKELETON_DATA;
	pColorImageFrame = new NUI_IMAGE_FRAME;
	if(SUCCEEDED(NuiCreateSensorByIndex(0, &sensor))){
		OutputDebugStringW(L"Connected to Kinect\n");
		if (SUCCEEDED(sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON | NUI_INITIALIZE_FLAG_USES_COLOR )))
		{
			OutputDebugStringW(L"Kinect with Skeleton\n");
		}
		else {
			OutputDebugStringW(L"Error initializing Kinect\n");
			exit(1);
		}
	} else {
		OutputDebugStringW(L"Error connecting to Kinect\n");
		exit(1);
	}
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &colorStreamHandle);
	OutputDebugStringW(L"Capture module Instantialized\n");
}

moduleCapture::~moduleCapture(){
	// Con los deletes hay un error con la liberacion de memoria :(

	//delete skeleton;
	//delete pSkeletonFrame;
	//delete pColorImageFrame;
}

point moduleCapture::SkeletonPointToScreen(Vector4 skeletonPoint, int height, int width)
{
	LONG x, y;
    USHORT depth;

    // Calculate the skeleton's position on the screen
    // NuiTransformSkeletonToDepthImage returns coordinates in NUI_IMAGE_RESOLUTION_320x240 space
    NuiTransformSkeletonToDepthImage(skeletonPoint, &x, &y, &depth);

    float screenPointX = static_cast<float>(x * width) / 320;
    float screenPointY = static_cast<float>(y * height) / 240;

	point p;
	p.x = screenPointX;
	p.y = screenPointY;
    return p;
}

HandsHip *moduleCapture::formHandsHip(bool rightHanded)
{
	NUI_SKELETON_DATA *skeleton = getSkeleton(); //Obtiene datos del esqueleto
	if(skeleton == NULL){  // Si no recibimos datos, retorna NULL
		return NULL;
	}
	HandsHip *h = new HandsHip;
	h->hipPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER];
	
	// Llenar los campos del handsHip con los datos del esqueleto teniendo en cuenta la lateralidad del usuario
	if(rightHanded){
		h->playingHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
		h->chordHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
	} else {
		h->playingHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
		h->chordHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
	}	
	return h;
}

ci::gl::Texture *moduleCapture::getNextFrame()
{	
	HRESULT hr = sensor->NuiImageStreamGetNextFrame(colorStreamHandle, 0, pColorImageFrame); // Obtiene el siguiente frame del kinect
	if (FAILED(hr)){
		return NULL; // Si habia error, retorna NULL
	}
	INuiFrameTexture *colorTexture = pColorImageFrame->pFrameTexture;
	NUI_LOCKED_RECT *colorRect = new NUI_LOCKED_RECT;
	colorTexture->LockRect( 0, colorRect, 0, 0 );
	if(colorRect->Pitch == 0){
		return NULL;
	}

	// Crea un gl::Texture con los datos del frame
	uint8_t *buffer = colorRect->pBits;
	int size        = resolution.width * resolution.height * 4;
	ci::gl::Texture *texture = new ci::gl::Texture(buffer, GL_BGRA, resolution.width, resolution.height);
	colorTexture->UnlockRect(0);

	// Libera el frame del kinect
	sensor->NuiImageStreamReleaseFrame(colorStreamHandle, pColorImageFrame);

	// Retorna la textura
	return texture;
}

NUI_SKELETON_DATA *moduleCapture::getSkeleton()
{
		// Obtiene el siguente frame con datos del esqueleto
		sensor->NuiSkeletonGetNextFrame(1000, pSkeletonFrame);

		// Filtra el resolutado para suavizar los movimientos
		sensor->NuiTransformSmooth(pSkeletonFrame, NULL);
		for(int i=0;i<NUI_SKELETON_COUNT;i++){
			// Busca los esqueletos que son seguidos
			skeleton = &pSkeletonFrame->SkeletonData[i];
			if(skeleton->eTrackingState == NUI_SKELETON_TRACKED)
			{
				// Retorna el primer esqueleto seguido
				return skeleton;
			}
		}
		// Si no encuentra esqueletos seguidos, retorna NULL
		return NULL;
}