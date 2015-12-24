#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cstdio>
#include <gl/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/freeglut.h>

class Camera
{
public:
	Camera();
	~Camera();

	/* 获得摄像机状态方法 */
	inline glm::vec3 getPosition()
    {
        return position;
    }
	inline glm::vec3 getView()
    {
        return view;
    }
	inline glm::vec3 getUpVector()
    {
        return upVector;
    }
	inline float getSpeed()
    {
        return speed;
    }

	/* 设置速度 */
	inline void setSpeed(float s)
	{
		speed = s;
	}

	/* 设置摄像机的位置, 观察点和向上向量 */
	void setCamera(float positionX, float positionY, float positionZ,
			 	   float viewX, float viewY, float viewZ,
				   float upVectorX, float upVectorY, float upVectorZ);

	/* 旋转摄像机方向 */
	void rotateView(float angle, float x, float y, float z);

	/* 根据鼠标设置摄像机观察方向 */
	void setViewFPS(int x, int y);
	void setViewRTS();

    /* 左右摄像机移动 */
	void yawCamera(float speed);

	/* 前后移动摄像机 */
	void moveCamera(float speed);

	inline void setUp(bool up)
	{
        upDown = up;
    }

	inline void setDown(bool down)
	{
        downDown = down;
    }

	inline void setLeft(bool left)
	{
        leftDown = left;
    }

	inline void setRight(bool right)
	{
        rightDown = right;
    }

	inline glm::mat4 &getViewMatrixClass()
	{
        return viewMatrix;
    }

    inline void setFlag(int f)
    {
        flag = f;
    }

    inline void setRotateXZ(float xz)
    {
        rotateXZ = xz;
    }

    inline void setRotateY(float y)
    {
        rotateY = y;
    }

    inline void setFovy(float f)
    {
        fovy = f;
    }

    inline float getFovy()
    {
        return fovy;
    }

    void printString()
    {

        glColor3f(1.0f, 1.0f, 1.0f);
        char pos[100];
        sprintf(pos, "Show the track of M I M U MOUSE ");
        //sprintf(pos, "(%.2f %.2f %.2f) (%.2f %.2f %.2f)\0\n", position.x, position.y, position.z, view.x, view.y, view.z);
        for (int i = 0; i < (int)strlen(pos); i++)
        {
            glRasterPos2f(-0.5f + i * 0.03f, 0.9f);
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15 , pos[i]);
        }

    }
    void update();
    inline void setWindowSize(int w, int h)
    {
        windowWidth = w;
        windowHeight = h;
    }

private:
    void computeMatrix();
	glm::vec3 position;      /* 位置 */
	glm::vec3 view;          /*  朝向 */
	glm::vec3 upVector;      /* 向上向量 */
	float speed;         /* 速度 */
	glm::mat4 projectionMatrix, viewMatrix, projectionInverseMatrix, viewInverseMatrix;
	bool upDown, downDown, leftDown, rightDown;
	int flag;
	float rotateXZ, rotateY;
	float fovy;
	int windowWidth;
	int windowHeight;
};

#endif
