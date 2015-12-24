#include "algorithm.h"
#include "kalman.h"
#define	Rad2Deg 	57.2957795130823208767981548141052
#include "vector.h"
//#include <arma/c_arma.h>
//using namespace std;
//vector<double> vectorAcc,vectorGyro;
float Q_angle  =  0.01; //0.001
float Q_gyro   =  0.0003;  //0.003
float R_angle  =  0.01;  //0.03

float x_bias = 0,x_angle = 0;
float P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0;
float  y, S;
float K_0, K_1;
float kalmanSingleAxisCalculate(float newAngle, float newRate,float dt)
{
// newAngle = angle measured with atan2 using the accelerometer
// newRate = angle measured using the gyro
// looptime = loop time in millis()
    x_angle = newRate - x_bias;
    P_00 +=  - dt * (P_10 + P_01) + Q_angle * dt;
    P_01 +=  - dt * P_11;
    P_10 +=  - dt * P_11;
    P_11 +=  + Q_gyro * dt;

    y = newAngle - x_angle;
    S = P_00 + R_angle;
    K_0 = P_00 / S;
    K_1 = P_10 / S;

    x_angle +=  K_0 * y;
    x_bias  +=  K_1 * y;
    P_00 -= K_0 * P_00;
    P_01 -= K_0 * P_01;
    P_10 -= K_1 * P_00;
    P_11 -= K_1 * P_01;

    return x_angle;
}
IMU kalman_filter_single_axis(IMU data)
{
    float anglem = data.gyro.x;
    printf("m:%f\t",anglem);
    //data.angle = kalmanSingleAxisCalculate(anglem,x_angle,data.T);
    //printf("%f\t",data.angle);
    return data;
}
IMU kalman_filter(IMU data)
{
    return data;
}
/*int arma()
{
	vectorAcc.push_back(1.0f);
	cArma* ca = new cArma(vectorAcc);
	double dPre  = ca->doPre();
}
*/
