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
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &colorStreamHandle);
	OutputDebugStringW(L"Capture module Instantialized\n");
}

moduleCapture::~moduleCapture(){
	//delete skeleton;
	//delete pSkeletonFrame;
	//delete pColorImageFrame;
	//delete colorSurface;
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

HandsHip *moduleCapture::formHandsHip(int playerId, bool rightHanded)
{
	NUI_SKELETON_DATA *skeleton = getSkeleton(playerId);
	if(skeleton == NULL){
		return NULL;
	}
	HandsHip *h = new HandsHip;
	
	if(rightHanded){
		h->hipPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER];
		h->playingHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
		h->chordHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
	} else {
		h->hipPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER];
		h->playingHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
		h->chordHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
	}	
	return h;
}

ci::gl::Texture *moduleCapture::getNextFrame()
{
	OutputDebugStringW(L"Next Frame start\n");
	
	HRESULT hr = sensor->NuiImageStreamGetNextFrame(colorStreamHandle, 0, pColorImageFrame);
	if (FAILED(hr)){
		OutputDebugStringW(L"No frame\n");
		return NULL;
	}
	OutputDebugStringW(L"Got frame\n");
	INuiFrameTexture * colorTexture = pColorImageFrame->pFrameTexture;
	NUI_LOCKED_RECT *colorRect = new NUI_LOCKED_RECT;
	//OutputDebugStringW(L"Got colorRect\n");
	colorTexture->LockRect( 0, colorRect, 0, 0 );
	if(colorRect->Pitch == 0){
		OutputDebugStringW(L"colorRect pitch 0\n");
		return NULL;
	}
	//OutputDebugStringW(L"colorRect pitch != 0\n");
	uint8_t *buffer = colorRect->pBits;
	int size        = resolution.width * resolution.height * 4;
	ci::gl::Texture *texture = new ci::gl::Texture(buffer, GL_BGRA, resolution.width, resolution.height);
	colorTexture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(colorStreamHandle, pColorImageFrame);
	return texture;
}

NUI_SKELETON_DATA *moduleCapture::getSkeleton(int playerId)
{
		// TODO Fix playerId
		sensor->NuiSkeletonGetNextFrame(1000, pSkeletonFrame);
		sensor->NuiTransformSmooth(pSkeletonFrame, NULL);
		for(int i=0;i<NUI_SKELETON_COUNT;i++){
			skeleton = &pSkeletonFrame->SkeletonData[i];
			if(skeleton->eTrackingState == NUI_SKELETON_TRACKED)
			{
				return skeleton;
			}
		}
		return NULL;
}