#include "stdafx.h"
#include "DataConvernt.h"
#include "kalman.h"
#include "SetCursorTest.h"
#include "COM.h"
#include "rs232.h"
std::vector<IMU> dataVec;
std::vector<IMUPOS> PosVec;
MouseCtrl Mouse;
FILE *Data;
float anglex,anglexm;
unsigned char datacount,alldatabyte[datalen];
short alldata[datalen],intacc[3],intgyro[3],intacci[3];
unsigned char buff;
bool checkFlag = true,FIRSTRUN = true;
IMUPOS MIMUpos;
IMU IMUdata,LastIMUdata;
IMUoffset IMUoff;
time_t starttime,endtime,nowtime;
int SCRWidth,SCRHeight;
extern char comNum;
extern char SerialMode;
void SerialGetData()
{
    if(SerialMode==0x01)
        ReceiveChar(&buff, 1);
    if(SerialMode==0x02)
        RS232_PollComport(comNum,&buff,1);
    if(buff == 0x80)
    {
        if(SerialMode==0x01)ReceiveChar(alldatabyte,datalen);
        if(SerialMode==0x02)RS232_PollComport(comNum,alldatabyte,datalen);
        for(unsigned int i=0;i<(datalen-keynum);i++)
            alldata[i]=alldatabyte[i];
        LastIMUdata = IMUdata;
        dataconvernt();
    }
}
IMUoffset IMUinit()
{
    Data=fopen("Data.txt","a");
    //fprintf(Data,"----------------NEW----------------\n");
    fclose(Data);
    unsigned char count = 0;
    for (;;)//init
    {
        if(SerialMode==0x01)
            ReceiveChar(&buff, 1);
        if(SerialMode==0x02)
            RS232_PollComport(comNum,&buff,1);
        if(buff == 0x80)
        {
            if(SerialMode==0x01)ReceiveChar(alldatabyte,datalen);
            if(SerialMode==0x02)RS232_PollComport(comNum,alldatabyte,datalen);
            for(unsigned int i=0;i<(datalen-keynum);i++)
                alldata[i]=alldatabyte[i];
            LastIMUdata = IMUdata;
            dataconvernt();
            if(FIRSTRUN)
            {
                FIRSTRUN = false;
                //if(!FIRSTRUN){}
                continue;
            }
            IMUoff.gyro += IMUdata.gyro;
            IMUoff.acc += IMUdata.acc;
            count++;
            if (count==3)
            {
                glm::vec3 zero(0.0,0.0,0.0);
                IMUoff.gyro = zero;
                IMUoff.acc = zero;
            }
            if(count == 10)
            {
                count = count - 3;
                break;
            }
        }
    }
    IMUoff.gyro /=(float)count;
    IMUoff.gravec = IMUoff.acc /(float)count;
    //IMUoff.gravec =  -AccCorrect(-IMUoff.gravec);
    IMUoff.gravec *= 9.8065f;
    printvec3("Gravec",IMUoff.gravec);
    printvec3("\nGyroOff",IMUoff.gyro);
    printf("\n");
    starttime = clock();
    SerialGetData();
    //if(abs(max(IMUdata.gyro-IMUoff.gyro))>0.05f)
        //IMUinit();
    return IMUoff;
}
void dataconvernt(void)
{
    unsigned char rate;
    //data convernt
    intacc[0] = ((alldata[0]) << 8) | alldata[1];
    intacc[1] = ((alldata[2]) << 8) | alldata[3];
    intacc[2] = ((alldata[4]) << 8) | alldata[5];
    intgyro[0] = ((alldata[6] << 8) | alldata[7]);
    intgyro[1] = ((alldata[8] << 8) | alldata[9]);
    intgyro[2] = ((alldata[10] << 8) | alldata[11]);
    intacci[0]= ((alldata[12]) << 8) | alldata[13];
    intacci[1] = ((alldata[14]) << 8) | alldata[15];
    intacci[2] = ((alldata[16]) << 8) | alldata[17];
    ///原始采样数据输出
    //printvec3("intacc",glm::vec3(intacckeyi[0],intacci[1],intacci[2]));
    //printvec3("intgyro",glm::vec3(intgyro[0],intgyro[1],intgyro[2]));
    //data compute
    IMUdata.pressure = ((alldata[18]) << 8) | alldata[19];
    rate = 2;
    IMUdata.acc.x = ((float)intacc[0]/(16384/rate));
    IMUdata.acc.y = ((float)intacc[1]/(16384/rate));
    IMUdata.acc.z = ((float)intacc[2]/(16384/rate));
    rate = 1;
    IMUdata.gyro.x = (float)intgyro[0]/(131/rate);
    IMUdata.gyro.y = (float)intgyro[1]/(131/rate);
    IMUdata.gyro.z = (float)intgyro[2]/(131/rate);//*Deg2Rad
    //IMUdata.gyro *= Rad2Deg;
    //IMUdata.gyro *= Rad2Deg;
    rate = 4;
    MIMUpos.acci.x = ((float)intacci[0]/(16384/rate));
    MIMUpos.acci.y = ((float)intacci[1]/(16384/rate));
    MIMUpos.acci.z = ((float)intacci[2]/(16384/rate));
    MIMUpos.acci *= 9.8065;

    //printf("acc:\t%f\t%f\t%f\t",IMUdata.acc.x,IMUdata.acc.y,IMUdata.acc.z);fprintf(Data,"%f\t%f\t%f\t",IMUdata.acc.x,IMUdata.acc.y,IMUdata.acc.z);
}
bool checkData()
{
    if( (abs(max(IMUdata.gyro))>500))//&(abs(max(IMUdata.gyro-LastIMUdata.gyro)>50.0f))) //检查陀螺仪
    {
        checkFlag = false;
        printf("g\n");
        return false;
    }
    if( (abs(max(IMUdata.acc))>20))//&(abs(max(IMUdata.acc-LastIMUdata.acc)>0.2f)))//检查加速度
    {
        checkFlag = false;
        printf("a\n");
        return false;
    }
    checkFlag = true;
    return true;
}
IMUPOS processing()
{
    bool dataGetCompete = false;
    while(!dataGetCompete)
    {
        if(SerialMode==0x01)ReceiveChar(&buff, 1);
        if(SerialMode==0x02)RS232_PollComport(comNum,&buff,1);
        if(buff == 0x80)
        {
            if(SerialMode==0x01)ReceiveChar(alldatabyte, datalen);
            if(SerialMode==0x02)RS232_PollComport(comNum,alldatabyte,datalen);
            for(unsigned int i=0;i<(datalen-keynum);i++)
                alldata[i]=0x00|alldatabyte[i];
            dataGetCompete = true;

            IMUdata.res = alldatabyte[20];
            IMUdata.key[2] = alldatabyte[21]&0x01;
            IMUdata.key[1] = alldatabyte[21]&0x02;
            endtime = clock();
            IMUdata.T = difftime(endtime,starttime)/1000;
            if (IMUdata.T==0) continue;
            starttime = endtime;
            //if (checkFlag==true)
            dataconvernt();
            IMUdata.acc = 9.8065f * IMUdata.acc ;
            if(!checkData())//如果返回为false，那么跳过本帧
                continue;
            ///--------Filter--------
            //IMUoff.gyro = glm::vec3(1.1097,1.0347,1.2856);
            IMUdata.gyro = IMUdata.gyro - IMUoff.gyro;
            LastIMUdata = IMUdata;
            //IMUdata.gyro = glm::vec3(0,0,0);
            IMUdata.gyro = IMUdata.gyro*0.3f + LastIMUdata.gyro*0.7f;//一阶低通滤波
            //IMUdata.acc =  -AccCorrect(-IMUdata.acc);
            //IMUdata.acc = IMUdata.acc*0.8f + LastIMUdata.acc*0.2f;//一阶低通滤波
            //IMUdata = kalman_filter_single_axis(IMUdata);
            //IMUdata.acc = glm::normalize(IMUdata.acc);
            if(FIRSTRUN)
            {
                computeInit(IMUdata,IMUoff,MIMUpos);
                FIRSTRUN = false;
                continue;
            }
            //MIMUpos = compute(IMUdata,IMUoff,MIMUpos);
            //vector 存储
            dataVec.push_back(IMUdata);
            if (dataVec.size()>10)
                dataVec.erase(dataVec.begin());
            /*PosVec.push_back(MIMUpos);
            if (PosVec.size()>10)
                PosVec.erase(PosVec.begin());*/
            //std::cout<<PosVec.at(0).acci.x<<std::endl;
            //把飞机模型数字仿真的结果直接输出给姿态解算模块，飞机模型使用的是苏联坐标系，解算使用的是欧美坐标系，需要注意
            //ahrs_step(  MIMUpos.angle, T,IMUdata.gyro[0], IMUdata.gyro[2],-IMUdata.gyro[1],IMUdata.acc[0], IMUdata.acc[2], IMUdata.acc[1],IMUdata.hmc[0],IMUdata.hmc[2],-IMUdata.hmc[1] );
            Mouse.getResKey(&dataVec);
            //print();///打印
            Mouse.GyroCtrlMouse(dataVec,SCRWidth,SCRHeight);
            //Mouse.AccCtrlMouse(IMUdata.acc[0],IMUdata.acc[1],SCRWidth,SCRHeight);
            //Mouse.AngleCtrlMouse(MIMUpos.angle.x,MIMUpos.angle.y,MIMUpos.angle.z);
           // Mouse.penCtrl(dataVec,PosVec);
            //Sleep(2);
        }
    }
    if (0)//(key)
    {
        MIMUpos.eulAngle=   glm::vec3(0,0,0);
        MIMUpos.angle   =   glm::vec3(0,0,0);
        MIMUpos.vel     =   glm::vec3(0,0,0);
        MIMUpos.pos     =   glm::vec3(0,0,0);
        IMUoff.acc      =   glm::vec3(0,0,0);
        IMUoff.gyro     =   glm::vec3(0,0,0);
        IMUoff.gravec   =   glm::vec3(0,0,0);
        IMUinit();
        FIRSTRUN = true;
    }
    return MIMUpos;
}
void print()
{
    Data=fopen("Data.txt","a");
    for(int i=0;i<3;i++)
        printf("%d\t",dataVec.back().key[i]);
    printvec3("ACC",IMUdata.acc);
    printvec3("ACCI",MIMUpos.acci);
    printvec3("GYRO",IMUdata.gyro);
    ///printvec3("HMC",IMUdata.hmc);
    //static glm::vec3 ang;
    //ang = ang + IMUdata.gyro*T;
    //printvec3("ANG",ang);
    ///printvec3("VEL",MIMUpos.vel);
    ///printvec3("POS",MIMUpos.pos);
    //printvec3("ANGLE",MIMUpos.angle);
    //for(int i=0;i<3;i++)printf("%d",dataVec.back().key[i]);printf("%t");
    printf("%dms\t",int(1000*IMUdata.T));fprintf(Data,"%f\t",IMUdata.T);
    printf("\n");fprintf(Data,"\n");
    fclose(Data);
}
void printvec3(char* str,glm::vec3 vec)
{
    printf("%s: %8.5f  %8.5f  %8.5f\t",str,vec.x,vec.y,vec.z);
    fprintf(Data,"%s\t%f\t%f\t%f\t",str,vec.x,vec.y,vec.z);
}
void printvec3(char* str,std::vector<unsigned char> keydata)
{
    int i= keydata.size();
    printf("%s:\t%d\t%d\t%d\t",str,keydata.at(i-3),keydata.at(i-2),keydata.at(i-1));
    fprintf(Data,"%s\t%d\t%d\t%d\t",str,keydata.at(i-3),keydata.at(i-2),keydata.at(i-1));
}
DWORD WINAPI MainProcess(PVOID dataVector)
{
    //GLmain(argc, argv);
    while(1)
    {
        processing();
    }
}
