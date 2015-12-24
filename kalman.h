#ifndef _KALMAN_H_
#define _KALMAN_H_

IMU kalman_filter(IMU data);
IMU kalman_filter_single_axis(IMU data);
/*float stateUpdate(const float gyro_m,float dt);
float kalmanUpdate(const float incAngle);*/
#endif
