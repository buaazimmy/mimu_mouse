#include "camera.h"

Camera::Camera()
{
	position = glm::vec3(0.0 , 0.0 , 0.0);
	view = glm::vec3(0.0, 0.0, -1.0);
	upVector = glm::vec3(0.0, 1.0, 0.0);
	speed = 0.5f;
	float fovy = 45;
}


Camera::~Camera()
{
}

void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		   float viewX, float viewY, float viewZ,
						   float upVectorX, float upVectorY, float upVectorZ)
{
	position = glm::vec3(positionX, positionY, positionZ);
	view = glm::vec3(viewX, viewY, viewZ);
	upVector = glm::vec3(upVectorX, upVectorY, upVectorZ);
}

/*  旋转摄像机方向  */
void Camera::rotateView(float angle, float x, float y, float z)
{
	glm::vec3 newView;

	/* 计算方向向量 */
	glm::vec3 tempView = view - position;

	/* 计算 sin 和cos值 */
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	/* 计算旋转向量的x值 */
	newView.x  = (cosTheta + (1 - cosTheta) * x * x) * tempView.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta) * tempView.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta) * tempView.z;

	/* 计算旋转向量的y值 */
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta) * tempView.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y) * tempView.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta) * tempView.z;

	/* 计算旋转向量的z值 */
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta) * tempView.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta) * tempView.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z) * tempView.z;

	/* 更新摄像机的方向 */
	view = position + newView;
}

/* 用鼠标旋转摄像机 */
void Camera::setViewFPS(int x, int y)
{

	int middleX = windowWidth / 2;   // 得到屏幕宽度的一半
	int middleY = windowHeight / 2;   // 得到屏幕高度的一半
	float angleY = 0.0f;							    // 摄像机左右旋转角度
	float angleX = 0.0f;		                        // 摄像机上下旋转角度
	static float currentRotX = 0.0f;

	// 如果鼠标没有移动,则不用更新
	if((x == middleX) && (y == middleY))
		return;

	if (flag == 1)
	{
        flag = 0;
        return;
    }

	// 得到鼠标移动方向
	angleY = (float)((middleX - x) ) / 1000.0f;
	angleX = (float)((middleY - y) ) / 1000.0f;

    static float lastRotX = 0.0f;      // 用于保存旋转角度
 	lastRotX = currentRotX;

	// 跟踪摄像机上下旋转角度
	currentRotX += angleX;

	// 如果上下旋转弧度大于1.0,我们截取到1.0并旋转
	if(currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		// 根据保存的角度旋转方向
		if(lastRotX != 1.0f)
		{
			// 通过叉积找到与旋转方向垂直的向量
			glm::vec3 vAxis = view - position;
			vAxis = glm::cross(vAxis, upVector);
			vAxis = glm::normalize(vAxis);

			//旋转
			rotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// 如果旋转弧度小于-1.0,则也截取到-1.0并旋转
	else if(currentRotX < -1.0f)
	{
		currentRotX = -1.0f;

		if(lastRotX != -1.0f)
		{

			// 通过叉积找到与旋转方向垂直的向量
			glm::vec3 vAxis = view - position;
			vAxis = glm::cross(vAxis, upVector);
			vAxis = glm::normalize(vAxis);


			rotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	else
	{
		// 找到与旋转方向垂直向量
		glm::vec3 vAxis = view - position;
		vAxis = glm::cross(vAxis, upVector);
		vAxis = glm::normalize(vAxis);

		rotateView(angleX, vAxis.x, vAxis.y, vAxis.z);
	}

	// 总是左右旋转摄像机
	rotateView(angleY, 0, 1, 0);

}

void Camera::setViewRTS()
{

	float angleY = 0.0f;							    // 摄像机左右旋转角度
	float angleX = 0.0f;		                        // 摄像机上下旋转角度
	static float currentRotXRTS = 0.0f;

	// 得到鼠标移动方向
	angleY = rotateY;
	angleX = rotateXZ;

    static float lastRotXRTS = 0.0f;      // 用于保存旋转角度
 	lastRotXRTS = currentRotXRTS;

	// 跟踪摄像机上下旋转角度
	currentRotXRTS += angleX;

	// 如果上下旋转弧度大于1.0,我们截取到1.0并旋转
	if(currentRotXRTS > 1.0f)
	{
		currentRotXRTS = 1.0f;

		// 根据保存的角度旋转方向
		if(lastRotXRTS != 1.0f)
		{
			// 通过叉积找到与旋转方向垂直的向量
			glm::vec3 vAxis = view - position;
			vAxis = glm::cross(vAxis, upVector);
			vAxis = glm::normalize(vAxis);

			//旋转
			rotateView(1.0f - lastRotXRTS, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// 如果旋转弧度小于-1.0,则也截取到-1.0并旋转
	else if(currentRotXRTS < -1.0f)
	{
		currentRotXRTS = -1.0f;

		if(lastRotXRTS != -1.0f)
		{

			// 通过叉积找到与旋转方向垂直的向量
			glm::vec3 vAxis = view - position;
			vAxis = glm::cross(vAxis, upVector);
			vAxis = glm::normalize(vAxis);


			rotateView(-1.0f - lastRotXRTS, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	else
	{
		// 找到与旋转方向垂直向量
		glm::vec3 vAxis = view - position;
		vAxis = glm::cross(vAxis, upVector);
		vAxis = glm::normalize(vAxis);

		rotateView(angleX, vAxis.x, vAxis.y, vAxis.z);
	}

	// 总是左右旋转摄像机
	rotateView(angleY, 0, 1, 0);
}


/* 左右移动摄像机 */
void Camera::yawCamera(float speed)
{
	glm::vec3 yaw;
	glm::vec3 cross = view - position;
	cross = glm::cross(cross, upVector);

	yaw = glm::normalize(cross);

	position.x += yaw.x * speed;
	position.z += yaw.z * speed;

	view.x += yaw.x * speed;
	view.z += yaw.z * speed;


}

/* 前后移动摄像机 */
void Camera::moveCamera(float speed)
{
	/* 计算方向向量 */
	glm::vec3 vector = view - position;
	vector = glm::normalize(vector);         /* 单位化 */

	/* 更新摄像机 */
	position.x += vector.x * speed;    /* 根据速度更新位置 */
	position.z += vector.z * speed;
	position.y += vector.y * speed;

	view.x += vector.x * speed;		 /* 根据速度更新方向 */
	view.y += vector.y * speed;
	view.z += vector.z * speed;
}

void Camera::update()
{

	if(upDown)   /* 向上方向键或'W'键按下 */
		moveCamera(speed);          /* 移动摄像机 */

	if(downDown) /* 向下方向键或'S'键按下 */
		moveCamera(-speed);         /* 移动摄像机 */

	if(leftDown) /* 向左方向键或'A'键按下 */
		yawCamera(-speed);          /* 移动摄像机 */

	if(rightDown) /* 向右方向键或'D'键按下 */
		yawCamera(speed);            /* 移动摄像机 */

	computeMatrix();
}

void Camera::computeMatrix()
{
    viewMatrix = glm::lookAt(position, view, upVector);
}
