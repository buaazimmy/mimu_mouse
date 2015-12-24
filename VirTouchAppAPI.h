#include <Windows.h>

#define ACTION_DATA_SIZE							48

#define VIRTOUCH_MASK_ForwardTap  0x00000001  //forward and back tap (left click)
#define VIRTOUCH_MASK_BackTap	  0x00000002  //back and forward tap (right click)
#define VIRTOUCH_MASK_DoubleTap	  0x00000004  //double tap (double click)
#define VIRTOUCH_MASK_Cross		  0x00000008  //cross gesture (Close window, Alt+F4)
#define VIRTOUCH_MASK_CircleCw	  0x00000010  //clockwise circular motion
#define VIRTOUCH_MASK_CircleCCw   0x00000020  //counter clockwise circular motion
#define VIRTOUCH_MASK_Shake		  0x00000040  //shake, reset gesture
#define VIRTOUCH_MASK_StayStill   0x00000080  //ring stays still at one location, used to show click menu
#define VIRTOUCH_MASK_FastTap	  0x00000100  //fast back and forward tap without prior stop, similar to BackTap (used to end drag&drop, zoom, and scroll modes)
#define VIRTOUCH_MASK_Touch		  0x00000200  //forward motion, (touch gesture, starts Drag&Drop)
#define VIRTOUCH_MASK_Lift		  0x00000400  //back motion, (used to end drag&drop, zoom, and scroll modes)
#define VIRTOUCH_MASK_SwipeRight  0x00000800
#define VIRTOUCH_MASK_SwipeUp	  0x00001000
#define VIRTOUCH_MASK_SwipeLeft   0x00002000
#define VIRTOUCH_MASK_SwipeDown   0x00004000
#define VIRTOUCH_MASK_MouseModeOn 0x00008000
#define VIRTOUCH_MASK_GameModeOn  0x00010000
#define VIRTOUCH_MASK_ApiModeOn   0x00020000

typedef struct
{
	UINT32 GestureFlags;
	BOOL tracked;
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float xCam;
	float yCam;
} SApiReport, *PSApiReport;

typedef struct
{
	unsigned char Command;
	unsigned char ActionId;
	unsigned char ActionData[ACTION_DATA_SIZE];
} VTDEV_IPC_MESSAGE, *PVTDEV_IPC_MESSAGE;

__declspec(dllexport) BOOL __stdcall VirTouchStartApiThread(void (__stdcall *VirTouchProcessiDeviceMessage)(PVTDEV_IPC_MESSAGE));

__declspec(dllexport) void __stdcall VirTouchStopAppThread(void);

__declspec(dllexport) void __stdcall VirTouchSetMouseMode();

__declspec(dllexport) void __stdcall VirTouchSetGameMode();

__declspec(dllexport) void __stdcall VirTouchSetApiMode();

__declspec(dllexport) BOOL __stdcall VirTouchParseApiReport(PSApiReport report, PVTDEV_IPC_MESSAGE msg);
