#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_
#include "glm/glm.hpp"
#include <iostream>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
// Computes the W component of the quaternion based on the X, Y, and Z components.
// This method assumes the quaternion is of unit length.
#define	Rad2Deg 	57.2957795130823208767981548141052
#define	Deg2Rad 	0.0174532922222222
#define  pi 3.1415926
struct IMU{
    glm::vec3 acc,gyro,hmc;
    int pressure;
    float T;
    unsigned char res,key[3];
};
struct IMUPOS{
    glm::vec3 acc,acci,vel,pos,angle,eulAngle;
    glm::quat q;
};
struct IMUoffset{
    glm::vec3 acc;
    glm::vec3 gyro;
    glm::vec3 gravec;
};
float max(glm::vec3 vec);
IMUPOS getFirstIMUPOS(IMU data);
IMUPOS getIMUPOS(IMU data);
IMUPOS compute(IMU data,IMUoffset IMUoff,IMUPOS oldPos);
void  computeInit(IMU data,IMUoffset IMUoff,IMUPOS Pos);
glm::vec3   AccCorrect(glm::vec3 accerr);
#endif

