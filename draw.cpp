#include "draw.h"
std::vector<POINT> CursorPos;
HWND hwndC = GetDesktopWindow();
HDC hdc = GetDC(hwndC);
unsigned char ResKeyMode;
void drawPlus(int x,int y,int r,int press)
    {

    PAINTSTRUCT ps;
    HPEN hPen;
    hPen = CreatePen( PS_SOLID , press , RGB( 255 , 0 , 0 ));
    SelectObject ( hdc , hPen );

    MoveToEx(hdc, (int) x - 10*r, (int) y, (LPPOINT) NULL);
    LineTo(hdc, (int) x + 10*r, (int) y);
    MoveToEx(hdc, (int) x, (int) y - 10*r, (LPPOINT) NULL);
    LineTo(hdc, (int) x, (int) y + 10*r);

    //ReleaseDC( hwndC , hdc );
}
void drawPix(std::vector<POINT> CursorPos)
{

    if(CursorPos.size()>3)
    {
        //for(int i =0;i<CursorPos.size();i++)
        {
            MoveToEx(hdc,CursorPos.at(CursorPos.size()-2).x, CursorPos.at(CursorPos.size()-2).y, (LPPOINT) NULL);
            LineTo(hdc, CursorPos.back().x,CursorPos.back().y);
        }
    }


        //SetPixelV(hdc,CursorPos.at(i).x,CursorPos.at(i).y,0xFFFF0000UL);//Color::Red
}
void ActionDraw(unsigned char key,POINT Cpoint,unsigned char mode)
{
    static time_t timenow = clock();
    static unsigned char lastDrawKey;
    static unsigned char i,j;
    i++;
    if(mode == LeftKeyMode)
    {
        if((key!=0)&(lastDrawKey==0))
        {
            mouse_event( MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0 );
            //Sleep(60);
            j=i;
            if(difftime(clock(),timenow)<400)
                return;
            timenow = clock();




            printf("%d\t",i);
        }
        if(key!=0)
        {
            //printf("%d\t",i);
        }
        if((key==0)&(lastDrawKey!=0))
        {
            if(j==(i-1))return;
            mouse_event( MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0 );
            printf("%d\t",i);
        }
    }
    if(mode == DrawMode)
    {
        if((key!=0)&(lastDrawKey==0))
        {
            CursorPos.clear();
            PAINTSTRUCT ps;
            HPEN hPen;
            hPen = CreatePen( PS_SOLID , 5 , RGB( 0 , 0 , 255 ));
            SelectObject ( hdc , hPen );
        }
        if(key!=0)
        {
            CursorPos.push_back(Cpoint);
            if (CursorPos.size()>10)
                CursorPos.erase(CursorPos.begin());
            drawPix(CursorPos);
        }
        if((key==0)&(lastDrawKey!=0))
        {
            //drawPlus(Cpoint.x,Cpoint.y,1,5);
        }
    }
    lastDrawKey = key;
}
void ActionRightKey(unsigned char key)
{
    static unsigned char lastRightKey;
    if((key!=0)&(lastRightKey==0))
    {
        mouse_event( MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0 );
    }
    if((key==0)&(lastRightKey!=0))
    {
        mouse_event( MOUSEEVENTF_LEFTUP, 0, 0, 0, 0 );
    }
    lastRightKey = key;
}
void AcitonModeKey(unsigned char key)
{
    static unsigned char lastModeKey;
    if((key!=0)&(lastModeKey==0))
    {
    }
    if((key==0)&(lastModeKey!=0))
    {
        ResKeyMode++;
        if(ResKeyMode>0x02)
            ResKeyMode=0x01;
        printf("%d\t\n",ResKeyMode);
    }
    lastModeKey = key;
}
DWORD WINAPI KeyAction(PVOID dataVector)
{
    std::vector<IMU>* dataVec = (std::vector<IMU>*)dataVector;
    POINT Cpoint;
    ResKeyMode = DrawMode;
    while(1)
    {
        GetCursorPos(&Cpoint);
        ///key1
        ActionDraw(dataVec->back().key[0],Cpoint,ResKeyMode);
        ///key2
        ActionRightKey(dataVec->back().key[1]);
        ///key3
        AcitonModeKey(dataVec->back().key[2]);

    }
}
