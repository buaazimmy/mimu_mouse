#ifndef _COM_H_
#define _COM_H_

void ComInit(unsigned char num);
bool openport(char *portname);
void ReceiveChar(unsigned char* RXBuff,int byte);
#endif
