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

/*  ��ת���������  */
void Camera::rotateView(float angle, float x, float y, float z)
{
	glm::vec3 newView;

	/* ���㷽������ */
	glm::vec3 tempView = view - position;

	/* ���� sin ��cosֵ */
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	/* ������ת������xֵ */
	newView.x  = (cosTheta + (1 - cosTheta) * x * x) * tempView.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta) * tempView.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta) * tempView.z;

	/* ������ת������yֵ */
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta) * tempView.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y) * tempView.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta) * tempView.z;

	/* ������ת������zֵ */
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta) * tempView.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta) * tempView.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z) * tempView.z;

	/* ����������ķ��� */
	view = position + newView;
}

/* �������ת����� */
void Camera::setViewFPS(int x, int y)
{

	int middleX = windowWidth / 2;   // �õ���Ļ��ȵ�һ��
	int middleY = windowHeight / 2;   // �õ���Ļ�߶ȵ�һ��
	float angleY = 0.0f;							    // �����������ת�Ƕ�
	float angleX = 0.0f;		                        // �����������ת�Ƕ�
	static float currentRotX = 0.0f;

	// ������û���ƶ�,���ø���
	if((x == middleX) && (y == middleY))
		return;

	if (flag == 1)
	{
        flag = 0;
        return;
    }

	// �õ�����ƶ�����
	angleY = (float)((middleX - x) ) / 1000.0f;
	angleX = (float)((middleY - y) ) / 1000.0f;

    static float lastRotX = 0.0f;      // ���ڱ�����ת�Ƕ�
 	lastRotX = currentRotX;

	// ���������������ת�Ƕ�
	currentRotX += angleX;

	// ���������ת���ȴ���1.0,���ǽ�ȡ��1.0����ת
	if(currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		// ���ݱ���ĽǶ���ת����
		if(lastRotX != 1.0f)
		{
			// ͨ������ҵ�����ת����ֱ������
			glm::vec3 vAxis = view - position;
			vAxis = glm::cross(vAxis, upVector);
			vAxis = glm::normalize(vAxis);

			//��ת
			rotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// �����ת����С��-1.0,��Ҳ��ȡ��-1.0����ת
	else if(currentRotX < -1.0f)
	{
		currentRotX = -1.0f;

		if(lastRotX != -1.0f)
		{

			// ͨ������ҵ�����ת����ֱ������
			glm::vec3 vAxis = view - position;
			vAxis = glm::cross(vAxis, upVector);
			vAxis = glm::normalize(vAxis);


			rotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	else
	{
		// �ҵ�����ת����ֱ����
		glm::vec3 vAxis = view - position;
		vAxis = glm::cross(vAxis, upVector);
		vAxis = glm::normalize(vAxis);

		rotateView(angleX, vAxis.x, vAxis.y, vAxis.z);
	}

	// ����������ת�����
	rotateView(angleY, 0, 1, 0);

}

void Camera::setViewRTS()
{

	float angleY = 0.0f;							    // �����������ת�Ƕ�
	float angleX = 0.0f;		                        // �����������ת�Ƕ�
	static float currentRotXRTS = 0.0f;

	// �õ�����ƶ�����
	angleY = rotateY;
	angleX = rotateXZ;

    static float lastRotXRTS = 0.0f;      // ���ڱ�����ת�Ƕ�
 	lastRotXRTS = currentRotXRTS;

	// ���������������ת�Ƕ�
	currentRotXRTS += angleX;

	// ���������ת���ȴ���1.0,���ǽ�ȡ��1.0����ת
	if(currentRotXRTS > 1.0f)
	{
		currentRotXRTS = 1.0f;

		// ���ݱ���ĽǶ���ת����
		if(lastRotXRTS != 1.0f)
		{
			// ͨ������ҵ�����ת����ֱ������
			glm::vec3 vAxis = view - position;
			vAxis = glm::cross(vAxis, upVector);
			vAxis = glm::normalize(vAxis);

			//��ת
			rotateView(1.0f - lastRotXRTS, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// �����ת����С��-1.0,��Ҳ��ȡ��-1.0����ת
	else if(currentRotXRTS < -1.0f)
	{
		currentRotXRTS = -1.0f;

		if(lastRotXRTS != -1.0f)
		{

			// ͨ������ҵ�����ת����ֱ������
			glm::vec3 vAxis = view - position;
			vAxis = glm::cross(vAxis, upVector);
			vAxis = glm::normalize(vAxis);


			rotateView(-1.0f - lastRotXRTS, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	else
	{
		// �ҵ�����ת����ֱ����
		glm::vec3 vAxis = view - position;
		vAxis = glm::cross(vAxis, upVector);
		vAxis = glm::normalize(vAxis);

		rotateView(angleX, vAxis.x, vAxis.y, vAxis.z);
	}

	// ����������ת�����
	rotateView(angleY, 0, 1, 0);
}


/* �����ƶ������ */
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

/* ǰ���ƶ������ */
void Camera::moveCamera(float speed)
{
	/* ���㷽������ */
	glm::vec3 vector = view - position;
	vector = glm::normalize(vector);         /* ��λ�� */

	/* ��������� */
	position.x += vector.x * speed;    /* �����ٶȸ���λ�� */
	position.z += vector.z * speed;
	position.y += vector.y * speed;

	view.x += vector.x * speed;		 /* �����ٶȸ��·��� */
	view.y += vector.y * speed;
	view.z += vector.z * speed;
}

void Camera::update()
{

	if(upDown)   /* ���Ϸ������'W'������ */
		moveCamera(speed);          /* �ƶ������ */

	if(downDown) /* ���·������'S'������ */
		moveCamera(-speed);         /* �ƶ������ */

	if(leftDown) /* ���������'A'������ */
		yawCamera(-speed);          /* �ƶ������ */

	if(rightDown) /* ���ҷ������'D'������ */
		yawCamera(speed);            /* �ƶ������ */

	computeMatrix();
}

void Camera::computeMatrix()
{
    viewMatrix = glm::lookAt(position, view, upVector);
}
