#include "stdafx.h"
#include "SetCursorTest.h"
#include "time.h"
#include "draw.h"
#include <stdlib.h>
CursorPosBuff CPosbuff;
int ScreenWidth,ScreenHeight;
int i;
unsigned char keyState;
unsigned char command;
time_t tstart,tend;
extern bool VirFlag;
/*#define MOUSEEVENTF_MOVE 0x00000001
#define MOUSEEVENTF_LEFTDOWN 0x00000002
#define MOUSEEVENTF_LEFTUP 0x00000004
#define MOUSEEVENTF_RIGHTDOWN 0x00000008
#define MOUSEEVENTF_RIGHTUP 0x00000010
#define MOUSEEVENTF_MIDDLEDOWN 0x00000020
#define MOUSEEVENTF_MIDDLEUP 0x00000040
#define MOUSEEVENTF_ABSOLUTE 0x00008000*/

int MouseCtrl::Setcursortest()
{
    for(;;)
    {

        printf("Enter your command:\n1.GetCursorPos\n2.SetCursorPos\n3.leftClick\n4.GetScreenMetric\n");
        command = getchar();
        switch(command)
        {
        case '1':
            for(;;)
            {
                POINT point;
                Sleep(100);
                GetCursorPos(&point);//获得鼠标位置
                //ScreenToClient(hwnd,&point);//将屏幕坐标转化为客户坐标
                printf("Your Cursor is in %d  X  %d\n",(int)point.x,(int)point.y);
            }
            break;
        case  '2':
            i = 100;
            for(;;)
            {
                tstart = clock();
                i++;
                SetCursorPos(i,i);//移动鼠标
                Sleep(100);
                tend =  clock();
                printf("%fms passed\n",difftime(tend,tstart));//计时
            }
            break;
        case '3':
            mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0 );//模拟鼠标左键单击
            Sleep(1000);
            break;
        case '4':
            ScreenWidth = GetSystemMetrics ( SM_CXSCREEN );
            ScreenHeight= GetSystemMetrics ( SM_CYSCREEN );
            printf("Your GetScreenMetric is %d  X  %d\n",ScreenWidth,ScreenHeight);
            break;
        case 'e':
            exit(1);
            break;
        }
        printf("\n\n");
    }
    return 0;
}
void LeftKeyCtr(unsigned char key)
{
    static unsigned char lastDrawKey;
    if((key!=0)&(lastDrawKey==0))
    {
        mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
        //Sleep(60);
    }
    if(key!=0)
    {
        //printf("%d\t",i);
    }
    if((key==0)&(lastDrawKey!=0))
    {
       // if(j==(i-1))return;
        mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
    }
    lastDrawKey = key;
}
void MouseCtrl::GyroCtrlMouse(std::vector<IMU> dataVec,int Width,int Height)
{
    float Wy,Wx,rate=0.8;
    Wy = dataVec.back().gyro.x;
    Wx = dataVec.back().gyro.z;
    //printf("%f\t",Wy);
    POINT Cpoint;
    GetCursorPos(&Cpoint);
    if(((int)Cpoint.x>=0)|((int)Cpoint.x<Width))
        CPosbuff.x = (int)(Wx/rate);
    if(((int)Cpoint.y>=0)|((int)Cpoint.y<Height))
        CPosbuff.y = (int)(Wy/rate);
    if(!VirFlag)mouse_event( MOUSEEVENTF_MOVE, (DWORD)CPosbuff.x, (DWORD)CPosbuff.y, 0, 0 );

    //LeftKeyCtr(dataVec.back().key[0]);
    //KeyAction(dataVec,Cpoint);
}
void MouseCtrl::AccCtrlMouse(float Ax,float Ay,int Width,int Height)
{
    POINT Cpoint;
    GetCursorPos(&Cpoint);
    /*if(((int)Cpoint.x>0)|((int)Cpoint.x<Width))
        CPosbuff.x = (int)(Ax*5);
    if(((int)Cpoint.y>0)|((int)Cpoint.y<Height))
        CPosbuff.y = (int)(Ay*5);*/
    CPosbuff.x = (int)(65536 * Ax);
    CPosbuff.y = (int)(65536 * Ay);
    mouse_event( MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE, (DWORD)CPosbuff.x, (DWORD)CPosbuff.y, 0, 0 );
}
void  MouseCtrl::AngleCtrlMouse(float x,float y,float z)
{
    x = -x;
    z = -z;
    long ax =( (2 * x + 180)/360)*65536;
    long ay = ((2 * y + 180)/360)*65536;
    long az = ((2 * z + 180)/360)*65536;

    CPosbuff.x = (int)(az%65536);
    CPosbuff.y = (int)(ax%65536);
    mouse_event( MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE, (DWORD)CPosbuff.x, (DWORD)CPosbuff.y, 0, 0 );
    if (ay>300)
    {
        mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
        mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
    }
    if (ay<60)
    {
        mouse_event( MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0 );
        mouse_event( MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0 );
    }

}
void  MouseCtrl::GetMetrics(int* Width,int* Height,int show)
{
    *Width = GetSystemMetrics ( SM_CXSCREEN );
    *Height= GetSystemMetrics ( SM_CYSCREEN );
    if(show)
        printf("Your GetScreenMetric is %d  X  %d\n",*Width,*Height);
}
void  MouseCtrl::writeplus(int scale)
{
    mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_MOVE, 16*scale, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_MOVE, -8*scale, 8*scale, 0, 0 );
    mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_MOVE, 0,-16*scale, 0, 0 );
    mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
}
void  MouseCtrl::writeSub(int scale)
{
    mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_MOVE, 16*scale, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
}
void  MouseCtrl::writemult(int scale)
{
    mouse_event( MOUSEEVENTF_MOVE, 0, -8*scale, 0, 0 );
    mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_MOVE, 16*scale, 16*scale, 0, 0 );
    mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_MOVE, -16*scale, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
    mouse_event( MOUSEEVENTF_MOVE, 16*scale,-16*scale, 0, 0 );
    mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
}
void keyAction(unsigned char * key)
{
    key=0;
}
void  MouseCtrl::getResKey(std::vector<IMU> * dataVec)
{
    int d=0;
    int len = dataVec->size()-1;
    if (len>5)
    {
        d = dataVec->at(len).res - dataVec->at(len  - 1).res;
        if (abs(d)>(dataVec->at(len).res/7))
        {
            if(d<0)
                keyState = 1;
            if(d>0)
                keyState = 0;
        }
        dataVec->at(len).key[0] = keyState;
    }
}
void  MouseCtrl::penCtrl(std::vector<IMU> dataVec,std::vector<IMUPOS> PosVec)
{
    int len = dataVec.size()-1;
    //printf("%d\t",dataVec.at(len).key[i]);
}
