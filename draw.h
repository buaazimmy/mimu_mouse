#ifndef _DRAW_H_
#define _DRAW_H_

#include "stdafx.h"
void drawPlus(int x,int y,int r,int press);
DWORD WINAPI KeyAction(PVOID dataVector);
#define DrawMode 0x01
#define LeftKeyMode 0x02
#endif
