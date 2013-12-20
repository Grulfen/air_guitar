#include "moduleCapture.h"
#include "string"

moduleCapture::moduleCapture()
{
	sensor = NULL;
	resolution.height = 480;
	resolution.width = 640;
	pSkeletonFrame = new NUI_SKELETON_FRAME;
	skeleton = new NUI_SKELETON_DATA;
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
	delete skeleton;
	delete pSkeletonFrame;
}

HandsHip *moduleCapture::formHandsHip(int playerId, bool rightHanded)
{
	NUI_SKELETON_DATA *skeleton = getSkeleton(playerId);
	if(skeleton == NULL){
		return NULL;
	}
	HandsHip *h = new HandsHip;
	h->hipPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER];
	if(rightHanded){
		h->playingHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
		h->chordHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
	} else {
		h->playingHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_LEFT];
		h->chordHandPosition = skeleton->SkeletonPositions[NUI_SKELETON_POSITION_WRIST_RIGHT];
	}	
	return h;
}

NUI_IMAGE_FRAME *moduleCapture::getNextFrame()
{
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 2, NULL, &colorStreamHandle);
	sensor->NuiImageStreamGetNextFrame(colorStreamHandle,1000, &pColorImageFrame);
	return &pColorImageFrame;
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