#include "moduleCapture.h"
#include "cinder/ImageIo.h"
#include "string"

moduleCapture::moduleCapture()
{
	sensor = NULL;
	resolution.height = 480;
	resolution.width = 640;
	pSkeletonFrame = new NUI_SKELETON_FRAME;
	skeleton = new NUI_SKELETON_DATA;
	pColorImageFrame = new NUI_IMAGE_FRAME;
	colorSurface = new ci::Surface8u;
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
	OutputDebugStringW(L"Capture module Instantialized\n");
}

moduleCapture::~moduleCapture(){
	//delete skeleton;
	//delete pSkeletonFrame;
	//delete pColorImageFrame;
	//delete colorSurface;
}

HandsHip *moduleCapture::formHandsHip(int playerId, bool rightHanded)
{
	NUI_SKELETON_DATA *skeleton = getSkeleton(playerId);
	if(skeleton == NULL){
		return NULL;
	}
	HandsHip *h = new HandsHip;
	
	if(rightHanded){
		h->hipPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_RIGHT];
		h->playingHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
		h->chordHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
	} else {
		h->hipPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT];
		h->playingHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
		h->chordHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
	}	
	return h;
}

void moduleCapture::getNextFrame(ci::Surface8u *surface)
{
	OutputDebugStringW(L"Next Frame start\n");
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &colorStreamHandle);
	sensor->NuiImageStreamGetNextFrame(colorStreamHandle,100, pColorImageFrame);
	INuiFrameTexture * colorTexture = pColorImageFrame->pFrameTexture;
	NUI_LOCKED_RECT *colorRect = new NUI_LOCKED_RECT;
	OutputDebugStringW(L"Got colorRect\n");
	// FIXME Here be dragons
	colorTexture->LockRect( 0, colorRect, 0, 0 );
	if(colorRect->Pitch == 0){
		OutputDebugStringW(L"colorRect pitch 0\n");
		return;
	}
	OutputDebugStringW(L"colorRect pitch != 0\n");
	uint8_t *buffer = colorRect->pBits;
	int32_t height        = surface->getHeight();
    int32_t width        = surface->getWidth();
    int32_t size        = width * height * 3;
	OutputDebugStringW(L"Before memcpy\n");
    memcpy(surface->getData(), buffer, size );
	delete colorRect;
	colorTexture->UnlockRect(0);
	OutputDebugStringW(L"After memcpy\n");
}

NUI_SKELETON_DATA *moduleCapture::getSkeleton(int playerId)
{
		// TODO Fix playerId
		sensor->NuiSkeletonGetNextFrame(1000, pSkeletonFrame);
		for(int i=0;i<NUI_SKELETON_COUNT;i++){
			skeleton = &pSkeletonFrame->SkeletonData[i];
			if(skeleton->eTrackingState == NUI_SKELETON_TRACKED)
			{
				return skeleton;
			}
		}
		return NULL;
}