#include "moduleCapture.h"

moduleCapture::moduleCapture()
{
	sensor = NULL;
	if(SUCCEEDED(NuiCreateSensorByIndex(0, &sensor))){
		std::cout << "Connected to Kinect" << std::endl;
		if (SUCCEEDED(sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON)))
		{
			std::cout << "Kinect with Skeleton" << std::endl;
			return;
		}
		else {
			std::cout << "Error initializing Kinect" << std::endl;
			exit(1);
		}
	}
	std::cout << "Error connecting to Kinect" << std::endl;
	exit(1);
}

HandsHip *moduleCapture::formHandsHip(int playerId, bool rightHanded)
{
	NUI_SKELETON_DATA *skeleton = &getSkeleton(playerId);
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

NUI_SKELETON_DATA moduleCapture::getSkeleton(int playerId)
{
		sensor->NuiSkeletonGetNextFrame(1000, pSkeletonFrame);
		for(int i=0;i<NUI_SKELETON_COUNT;i++){
			const NUI_SKELETON_DATA &skel = pSkeletonFrame->SkeletonData[i];
			if(skel.eTrackingState == NUI_SKELETON_TRACKED)
			{
				return skel;
			}
		}
}