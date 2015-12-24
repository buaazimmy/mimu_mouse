// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _STDAFX_H_
#define _STDAFX_H_
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include "algorithm.h"
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <Gdiplus.h>
#include <tchar.h>
/*#include "rs232.h"*/
#define baudrate 38400
#define BTmode 0x01
#define CDCmode 0x02
DWORD WINAPI InitMIMU(PVOID dataVector);

#endif
