// AppAPITest.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
extern "C"
{
#include "VirTouchAppAPI.h"
}
#include "AppAPITest.h"
bool VirFlag;
UINT32 lastDetectedGesture;
int gestureId = 0;
int reconnectCount = 0;

void __stdcall VirTouchMessageProcessing(PVTDEV_IPC_MESSAGE pInMsg)
{
	VTDEV_IPC_MESSAGE msg = *pInMsg;
    SApiReport report;

	//system("cls");

	if (VirTouchParseApiReport(&report, &msg))
	{
/*		if (report.GestureFlags & VIRTOUCH_MASK_MouseModeOn) printf("Mouse Mode On\n");
		if (report.GestureFlags & VIRTOUCH_MASK_GameModeOn)  printf("Vir Mode On\n");
		if (report.GestureFlags & VIRTOUCH_MASK_ApiModeOn) printf("API Mode On\n");
        VirFlag = report.tracked;
		if (report.tracked)
		{
			//Ring center 3D xyz coordinates are copmuted in inches, multiplying by 2.54 converts it to cm
			printf("RingPosition_xyz   = [%3.2f  %3.2f  %3.2f] cm\n", report.x * 2.54, report.y * 2.54, report.z * 2.54);

			//Ring norm vector: vector perpendicular to the plane of the ring
			printf("RingNorm_xyz       = [%3.2f  %3.2f  %3.2f]\n", report.nx, report.ny, report.nz);

			//Ring center position in pixels in VirTouch camera, camera resolution [800, 600]
			printf("RingCamPosition_xy = [%3.2f  %3.2f] pixels\n", report.xCam, report.yCam);

			if (report.GestureFlags & VIRTOUCH_MASK_ForwardTap)	{ lastDetectedGesture = VIRTOUCH_MASK_ForwardTap;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_BackTap)	{ lastDetectedGesture = VIRTOUCH_MASK_BackTap;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_DoubleTap)	{ lastDetectedGesture = VIRTOUCH_MASK_DoubleTap;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_Cross)		{ lastDetectedGesture = VIRTOUCH_MASK_Cross;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_CircleCw)	{ lastDetectedGesture = VIRTOUCH_MASK_CircleCw;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_CircleCCw)	{ lastDetectedGesture = VIRTOUCH_MASK_CircleCCw;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_Shake)		{ lastDetectedGesture = VIRTOUCH_MASK_Shake;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_StayStill)	{ lastDetectedGesture = VIRTOUCH_MASK_StayStill;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_FastTap)	{ lastDetectedGesture = VIRTOUCH_MASK_FastTap;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_Touch)		{ lastDetectedGesture = VIRTOUCH_MASK_Touch;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_Lift)		{ lastDetectedGesture = VIRTOUCH_MASK_Lift;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_SwipeRight)	{ lastDetectedGesture = VIRTOUCH_MASK_SwipeRight;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_SwipeUp)	{ lastDetectedGesture = VIRTOUCH_MASK_SwipeUp;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_SwipeLeft)	{ lastDetectedGesture = VIRTOUCH_MASK_SwipeLeft;  gestureId++; }
			if (report.GestureFlags & VIRTOUCH_MASK_SwipeDown)	{ lastDetectedGesture = VIRTOUCH_MASK_SwipeDown;  gestureId++; }

			if (lastDetectedGesture == VIRTOUCH_MASK_ForwardTap) printf("Last Gesture(%d): ForwardTap\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_BackTap)	 printf("Last Gesture(%d): BackTap\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_DoubleTap)	 printf("Last Gesture(%d): DoubleTap\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_Cross)		 printf("Last Gesture(%d): Cross\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_CircleCw)	 printf("Last Gesture(%d): Clockwise Circle\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_CircleCCw)  printf("Last Gesture(%d): Counter Clockwise Circle\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_Shake)		 printf("Last Gesture(%d): Shake\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_StayStill)  printf("Last Gesture(%d): StayStill\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_FastTap)	 printf("Last Gesture(%d): FastTap\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_Touch)		 printf("Last Gesture(%d): Touch\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_Lift)		 printf("Last Gesture(%d): Lift\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_SwipeRight) printf("Last Gesture(%d): SwipeRight\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_SwipeUp)	 printf("Last Gesture(%d): SwipeUp\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_SwipeLeft)  printf("Last Gesture(%d): SwipeLeft\n", gestureId);
			if (lastDetectedGesture == VIRTOUCH_MASK_SwipeDown)  printf("Last Gesture(%d): SwipeDown\n", gestureId);

			printf("Pipe reconnect count %d\n", reconnectCount);
		}
		else
		{
			printf("Ring is not tracked\n");
		}
	}
	else if (msg.Command == 200)
	{
		reconnectCount++;
		printf("Message Pipe reconnect %d\n", reconnectCount);
	}
	else
	{
		printf("Unrecognized report\n");*/
	}
}
bool VirInit(void)
{
	int res = VirTouchStartApiThread(VirTouchMessageProcessing);
	if (res != TRUE)
	{
		printf("Can not connect to Virtouch.\n");
		VirFlag = false;
		return VirFlag;
	}
    VirTouchSetGameMode();
}
 int wwmain(int argc, _TCHAR* argv[])
{

	int res = VirTouchStartApiThread(VirTouchMessageProcessing);

	//VirTouchSetApiMode();

	if (res != TRUE)
	{
		printf("Can not connect to device.\n");
		printf("hit enter to quit.\n");
	}

	int ch = _getch();
	while (ch != 10)
	{
		if (ch == (int)'G' || ch == (int)'g')
		{
			//In Game Mode virtouch only moves mouse pointer, responds to shake gesture, and responds to Alt key presses (if VirTouch Tray App is running). All other gestures are not detected.
			VirTouchSetGameMode();
		}
		if (ch == (int)'M' || ch == (int)'m')
		{
			//Normal mode of operation when user can control mouse pointer and gestures are detected and executed accoding to operation logic.
			VirTouchSetMouseMode();
		}
		if (ch == (int)'A' || ch == (int)'a')
		{
			//Api Mode is for 3rd party applications and games development. VirTouch does not control mouse pointer and does not execute detected gestures. All gestures are detected and information about detected gestures is passed in ApiReport structure (see callback function VirTouchMessageProcessing).
			VirTouchSetApiMode();
		}
		if (ch == 10) //Enter
		{
			break;
		}
		ch = _getch();
		//getc(stdin);
	}

	//if ApiMode is not turned off VirTouch will automatically switch to MouseMode 3 seconds after application breaks api messaging pipe
	VirTouchSetMouseMode();

	VirTouchStopAppThread();

	return 0;
}
