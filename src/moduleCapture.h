#ifndef MODULECAPTURE_H
#define MODULECAPTURE_H

#include <Windows.h>
#include <NuiApi.h>
#include <iostream>
#include <vector>


struct resolution_t{
	int width;
	int height;
};

struct HandsHip{
	Vector4 hipPosition;
	Vector4 playingHandPosition;
	Vector4 chordHandPosition;
};

class moduleCapture
{
public:
	moduleCapture();
	int playerId;
	resolution_t resolution;
	HandsHip *formHandsHip(int playerId, bool rightHanded=true);
	NUI_IMAGE_FRAME *getNextFrame();
	INuiSensor *sensor;
private:
	HANDLE colorStreamHandle;
	NUI_IMAGE_FRAME pColorImageFrame;
	NUI_SKELETON_FRAME *pSkeletonFrame;
	NUI_SKELETON_DATA *getSkeleton(int playerID);
};

#endif