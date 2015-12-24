#ifndef _SETCURSORTEST_H
#define _SETCURSORTEST_H
#include "DataConvernt.h"
//#include "time.h"
#include <iostream>
#include <stdio.h>
#include <windows.h>

struct CursorPosBuff{
        int x,y;
        };
class MouseCtrl{
    public:
        void AngleCtrlMouse(float x,float y,float z);
        void GyroCtrlMouse(std::vector<IMU> dataVec,int Width,int Height);
        void AccCtrlMouse(float Ax,float Ay,int Width,int Height);
        void GetMetrics(int * Width,int * Height,int show);
        void writeplus(int scale);
        void writeSub(int scale);
        void writemult(int scale);
        void penCtrl(std::vector<IMU> dataVec,std::vector<IMUPOS> PosVec);
        void getResKey(std::vector<IMU> * dataVec);
    private:
        int Setcursortest(void);

};
#endif
