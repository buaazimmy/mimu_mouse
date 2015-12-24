#include "stdafx.h"
#include <QtGui>
#include "MIMUGUI.h"
#include "GL.h"
#include "DataConvernt.h"
#include "SetCursorTest.h"
#include "COM.h"
#include "rs232.h"
#include <Gdiplus.h>
#include "AppAPITest.h"
#include "VirTouchAppAPI.h"
#include "draw.h"

char SerialMode = BTmode;//Choose MIMU MOUSE Serial Work Mode: BTmode or CDCmode
DWORD WINAPI InitMIMU(PVOID dataVector)
{
    char* num = (char*)dataVector;
    extern std::vector<IMU> dataVec;
    printf("InitMIMU Start\n");
    if(SerialMode==BTmode)ComInit(*num);
    if(SerialMode==CDCmode)CDCComInit(*num);
    IMUinit();
    printf("IMUInit Success\n");
    VirInit();//VirTouch Initial
    processing();
    HANDLE Process = CreateThread(NULL,0,MainProcess,0,0,NULL);
    HANDLE hThread = CreateThread(NULL,0,KeyAction,&dataVec,0,NULL);
    printf("InitMIMU Success\n");
}
int main(int argc, char *argv[])
{
    QApplication MIMUGUI(argc,argv);
    myclass newc;
    newc.show();
    MIMUGUI.setQuitOnLastWindowClosed(false);
    return MIMUGUI.exec();
   /* CloseHandle(InitMouse);
    CloseHandle(hThread);
    CloseHandle(Process);*/
}

