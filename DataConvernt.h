#ifndef _DATACONVERNT_H_
#define _DATACONVERNT_H_
#include "algorithm.h"
#define datalen 22
#define keynum 2
void dataconvernt(void);
IMUPOS processing(void);
bool checkData(void);
void print(void);
void printvec3(char* str,glm::vec3 vec);
IMUoffset IMUinit(void);
void printvec3(char* str,std::vector<unsigned char> keydata);
DWORD WINAPI MainProcess(PVOID dataVector);
#endif
