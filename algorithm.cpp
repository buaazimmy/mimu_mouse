#include "stdafx.h"
#include <iostream>
#include "algorithm.h"
#include "DataConvernt.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
//glm::vec3   Amat1(-0.00999461225,0.01145208000,0.05088829300);
//glm::mat3x3 Amat2(0.99687037050,-0.00895439650,0.05762327200,0.00772537300,0.99866786150,0.00295365300,-0.07352995750,-0.00785416350,0.99715167500);
//glm::mat3x3 Amat3(0.00709474575,0,0,0,-0.01151806550,0,0,0,-0.051498884);
glm::vec3   Amat1( 0.0034,-0.0097,-0.0171);
glm::mat3x3 Amat2( 0.9706 ,-0.0217 ,-0.2303,    0.0262  ,  0.9881  ,  0.0025,    0.2307  , -0.0094  ,  0.9722);
glm::mat3x3 Amat3( 0.0009    ,     0     ,    0,         0 ,  -0.0027   ,      0,         0 ,        0 ,  -0.0044);
/**< 求最大值 */
float max(glm::vec3 vec)
{
    float max;
    max = abs(vec.x) > abs(vec.y) ? abs(vec.x) : abs(vec.y);
    max = max > abs(vec.z) ? max : abs(vec.z);
    return max;
}
glm::vec3   AccCorrect(glm::vec3 accerr)
{
    glm::vec3   acc;
    glm::vec3   sqacc(accerr.x*accerr.x,accerr.y*accerr.y,accerr.z*accerr.z);
    acc = Amat1 + Amat2*accerr + Amat3*sqacc;
    return acc;
}
glm::vec3 GetInitEuler(glm::vec3 gravity) {
    glm::vec3 InitAngle;
    // pitch: (nose up/down, about Y axis)
    InitAngle.y = atan(gravity.x / sqrt(gravity.y*gravity.y + gravity.z*gravity.z));
    // roll: (tilt left/right, about X axis)
    InitAngle.x = atan(gravity.y / sqrt(gravity.x*gravity.x + gravity.z*gravity.z));
    return InitAngle;
}
glm::quat getquat(glm::vec3 ang)
{
    glm::quat quater;
    quater.x = cos(ang.x/2)*cos(ang.y/2)*cos(ang.z/2) - sin(ang.x/2)*sin(ang.y/2)*sin(ang.z/2);
    quater.y = sin(ang.x/2)*cos(ang.y/2)*cos(ang.z/2) - cos(ang.x/2)*sin(ang.y/2)*sin(ang.z/2);
    quater.z = cos(ang.x/2)*sin(ang.y/2)*cos(ang.z/2) + sin(ang.x/2)*cos(ang.y/2)*sin(ang.z/2);
    quater.w = cos(ang.x/2)*cos(ang.y/2)*sin(ang.z/2) + sin(ang.x/2)*sin(ang.y/2)*cos(ang.z/2);
    return quater;
}
void computeInit(IMU data,IMUoffset IMUoff,IMUPOS Pos)
{
    /*angle.x = -asin(gra.y/sqrt(gra.y*gra.y+gra.z+gra.z));
    angle.y = asin(gra.x/sqrt(gra.x*gra.x+gra.z+gra.z));
    angle.z = 0;*/
    /*float G = sqrt(gra.x*gra.x+gra.y*gra.y+gra.z*gra.z);
    angle.x = acos(gra.x/G);
    angle.y = acos(gra.y/G);
    angle.z = acos(gra.z/G);*/
    Pos.angle = data.gyro * data.T;
    Pos.angle *=Deg2Rad;
    Pos.q = getquat(Pos.angle);
    Pos.q = glm::normalize(Pos.q);
}
IMUPOS compute(IMU data,IMUoffset IMUoff,IMUPOS oldPos)
{
    IMUPOS MIMUpos;
    float u,ac,as;
    MIMUpos.acc   = data.acc;
    //MIMUpos.acci = oldPos.q*data.acc - IMUoff.gravec;
    MIMUpos.acci = oldPos.acci;
    MIMUpos.vel = MIMUpos.acci * data.T + oldPos.vel;
    MIMUpos.pos = MIMUpos.vel * data.T + oldPos.pos;
    MIMUpos.angle = data.gyro * data.T;
    MIMUpos.angle *=Deg2Rad;
    u = sqrt(MIMUpos.angle.x*MIMUpos.angle.x+MIMUpos.angle.y*MIMUpos.angle.y+MIMUpos.angle.z*MIMUpos.angle.z);
    ac = cos(u/2);
    as = sin(u/2)/u;
    glm::quat rk(ac,as*MIMUpos.angle.x,as*MIMUpos.angle.y,as*MIMUpos.angle.z);
    MIMUpos.eulAngle = glm::eulerAngles(oldPos.q);
    MIMUpos.q=oldPos.q*rk;
    MIMUpos.q = glm::normalize(MIMUpos.q);
    MIMUpos.angle.x = glm::roll(MIMUpos.q);
    MIMUpos.angle.y = glm::pitch(MIMUpos.q);
    MIMUpos.angle.z = glm::yaw(MIMUpos.q);

    return MIMUpos;
}
