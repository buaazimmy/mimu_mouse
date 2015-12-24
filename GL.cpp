#include "stdafx.h"
#include "camera.h"
#include "DataConvernt.h"
#include <vector>

std::vector<IMUPOS> GLpos;
int windowWidth = 800, windowHeight = 600;
bool isFullScreen = false;
glm::mat4 cameraProjectionMatrix, cameraViewMatrix;
glm::ivec2 lastPassiveMove;
Camera GLcamera;
glm::vec3 BoxPoint[8];
void processNormalKeys(GLubyte key, int x, int y)
{
    if (key == 27)
    {
        exit(0);
    }

    switch (key)
    {
        case 'a':
        case 'A':
            GLcamera.setLeft(true);
            break;

        case 'd':
        case 'D':
            GLcamera.setRight(true);
            break;

        case 'w':
        case 'W':
            GLcamera.setUp(true);
            break;

        case 's':
        case 'S':
            GLcamera.setDown(true);
            break;
    }
}
void processSpecialKeys(int key, int x, int y)
{

    switch (key)
    {

        case GLUT_KEY_F3:
        if (!isFullScreen)
        {
            glutFullScreen();
            isFullScreen = true;
        }
        else
        {
            glutPositionWindow(50, 50);
            glutReshapeWindow(800, 600);
            isFullScreen = false;
        }
        break;
    }
}
void releaseNormalKey(GLubyte key, GLint x, GLint y)
{
    switch (key)
    {
        case 'a':
        case 'A':
            GLcamera.setLeft(false);
            break;

        case 'd':
        case 'D':
            GLcamera.setRight(false);
            break;

        case 'w':
        case 'W':
            GLcamera.setUp(false);
            break;

        case 's':
        case 'S':
            GLcamera.setDown(false);
            break;
    }
}

void releaseSpecialKey(GLint key, GLint x, GLint y)
{

}
void processMouse(int button, int state, int x, int y)
{

}

void processMouseWheel(int button, int dir, int x, int y)
{

}

void mouseMove(int x, int y)
{
    float deltaAngle, deltaAngleY;

    if (x < 0)
        deltaAngle = 0;
    else if (x > windowWidth)
            deltaAngle = 0;
    else if (x < 10)
         {
             deltaAngle = 0.0001 * (x - windowWidth / 2);
         }
    else if (x > windowWidth - 10)
         {
             deltaAngle = 0.0001 * (x - windowWidth / 2);
         }
    else
        deltaAngle = 0;

    GLcamera.setRotateY(-deltaAngle);

    if (y < 0)
        deltaAngleY = 0;
    else if (y > windowHeight)
            deltaAngleY = 0;
    else if (y < 10)
            deltaAngleY = -0.0001 * (y - windowHeight / 2);
    else if (y > windowHeight - 10)
            deltaAngleY = -0.0001 * (y - windowHeight / 2);
    else
        deltaAngleY = 0;

    GLcamera.setRotateXZ(deltaAngleY);
}

void processMouseActiveMotion(int x, int y)
{
    mouseMove(x, y);

	if (lastPassiveMove.y - y > 0)
		printf("+");
	else
		printf("-");

	lastPassiveMove.x = x;
	lastPassiveMove.y = y;
}

void processMousePassiveMotion(int x, int y)
{
    mouseMove(x, y);
	lastPassiveMove.x = x;
	lastPassiveMove.y = y;
}
void initGL()
{
	GLcamera.setCamera(0, 0, 5, 0, 0, 0, 0, 1, 0);
	GLcamera.setSpeed(0.05);
    GLcamera.update();

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0 * windowWidth / windowHeight, 0.1, 1000.0f);
    glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(cameraProjectionMatrix));

    glPointSize(3.0f);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

}
void printString(char* strings,float x,float y)
{

    glColor3f(1.0f, 1.0f, 1.0f);
    //sprintf(pos, "(%.2f %.2f %.2f) (%.2f %.2f %.2f)\0\n", position.x, position.y, position.z, view.x, view.y, view.z);
    for (int i = 0; i < (int)strlen(strings); i++)
    {
        //printf("%s",strings);
        glRasterPos2f(x + i * 0.03f, y);
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15 , *(strings+i));
    }
}
void cameraprint(IMUPOS Mpos,float x,float y)
{
    char posStr[100];
    sprintf(posStr, "Vel:(%.2f %.2f %.2f) pos:(%.2f %.2f %.2f)\0\n", Mpos.vel.x,Mpos.vel.y,Mpos.vel.z,Mpos.pos.x, Mpos.pos.y, Mpos.pos.z);
    printString(posStr,x,y);
    sprintf(posStr, "acc:(%.2f %.2f %.2f) acci:(%.2f %.2f %.2f)\0\n", Mpos.acc.x, Mpos.acc.y, Mpos.acc.z, Mpos.acci.x, Mpos.acci.y, Mpos.acci.z);
    printString(posStr,x,y+0.1);
    sprintf(posStr, "Roll %.2f Pitch %.2f Yaw %.2f\0\n", Mpos.angle.x, Mpos.angle.y, Mpos.angle.z);//, Mpos.eulAngle.x, Mpos.eulAngle.y, Mpos.eulAngle.z);
    printString(posStr,x,y+0.2);

}
void drawBox(IMUPOS pos,float boxSize)
{
    BoxPoint[0]=glm::vec3(-1.0f, 1.0f, 1.0f);
    BoxPoint[1]=glm::vec3( 1.0f, 1.0f, 1.0f);
    BoxPoint[2]=glm::vec3( 1.0f,-1.0f, 1.0f);
    BoxPoint[3]=glm::vec3(-1.0f,-1.0f, 1.0f);
    BoxPoint[4]=glm::vec3(-1.0f, 1.0f,-1.0f);
    BoxPoint[5]=glm::vec3(-1.0f,-1.0f,-1.0f);
    BoxPoint[6]=glm::vec3( 1.0f,-1.0f,-1.0f);
    BoxPoint[7]=glm::vec3( 1.0f, 1.0f,-1.0f);
    for(int i=0;i<8;i++)
    {
        BoxPoint[i]=pos.q * BoxPoint[i];
        //BoxPoint[i]=boxSize*BoxPoint[i];
        //BoxPoint[i] += pos.pos;
    }
    //绘制
    glBegin(GL_QUADS);
    // 前面
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(BoxPoint[0].x, BoxPoint[0].y,BoxPoint[0].z);
    glVertex3f(BoxPoint[1].x, BoxPoint[1].y,BoxPoint[1].z);
    glVertex3f(BoxPoint[2].x, BoxPoint[2].y,BoxPoint[2].z);
    glVertex3f(BoxPoint[3].x, BoxPoint[3].y,BoxPoint[3].z);

    // 左面
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(BoxPoint[4].x, BoxPoint[4].y,BoxPoint[4].z);
    glVertex3f(BoxPoint[5].x, BoxPoint[5].y,BoxPoint[5].z);
    glVertex3f(BoxPoint[3].x, BoxPoint[3].y,BoxPoint[3].z);
    glVertex3f(BoxPoint[0].x, BoxPoint[0].y,BoxPoint[0].z);

    // 右面
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(BoxPoint[1].x, BoxPoint[1].y,BoxPoint[1].z);
    glVertex3f(BoxPoint[2].x, BoxPoint[2].y,BoxPoint[2].z);
    glVertex3f(BoxPoint[6].x, BoxPoint[6].y,BoxPoint[6].z);
    glVertex3f(BoxPoint[7].x, BoxPoint[7].y,BoxPoint[7].z);

    // 后面
    glColor3f(0.5f, 0.5f, 1.0f);
    glVertex3f(BoxPoint[7].x, BoxPoint[7].y,BoxPoint[7].z);
    glVertex3f(BoxPoint[6].x, BoxPoint[6].y,BoxPoint[6].z);
    glVertex3f(BoxPoint[5].x, BoxPoint[5].y,BoxPoint[5].z);
    glVertex3f(BoxPoint[4].x, BoxPoint[4].y,BoxPoint[4].z);

    // 上面
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(BoxPoint[4].x, BoxPoint[4].y,BoxPoint[4].z);
    glVertex3f(BoxPoint[0].x, BoxPoint[0].y,BoxPoint[0].z);
    glVertex3f(BoxPoint[1].x, BoxPoint[1].y,BoxPoint[1].z);
    glVertex3f(BoxPoint[7].x, BoxPoint[7].y,BoxPoint[7].z);

    // 下面
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex3f(BoxPoint[2].x, BoxPoint[2].y,BoxPoint[2].z);
    glVertex3f(BoxPoint[3].x, BoxPoint[3].y,BoxPoint[3].z);
    glVertex3f(BoxPoint[5].x, BoxPoint[5].y,BoxPoint[5].z);
    glVertex3f(BoxPoint[6].x, BoxPoint[6].y,BoxPoint[6].z);
    glEnd();
}
void drawVec3(glm::vec3 vec)
{
        glBegin(GL_LINES);
        glVertex3f(0.0f,0.0f,0.0f);
        glVertex3f(vec.x,vec.y,vec.z);
        glEnd();
}
void renderScene()
{
    IMUPOS Mpos;
    Mpos = processing();

    glm::mat3 normalMatrix = glm::mat3(glm::lookAt(glm::vec3(0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	glm::mat3 upMatrix = glm::mat3(glm::lookAt(glm::vec3(0.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	glm::mat3 rotateMatrix = glm::inverse(upMatrix) * normalMatrix;


    GLcamera.setViewRTS();
    GLcamera.update();

    cameraViewMatrix = GLcamera.getViewMatrixClass();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(cameraProjectionMatrix));

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(cameraViewMatrix));

    glViewport(0, 0, windowWidth, windowHeight);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        ///绘制运动轨迹
 //   glBegin(GL_LINES);
    /*    GLpos.push_back(Mpos.pos/10.0f);
    	glBegin(GL_LINES);
		for(int i=0;i<GLpos.size()-1;i++)
        {
            //glColor3f(i%3,(i+1)%3,(i+2)%3);
            //glVertex3f(0.0f,0.0f,0.0f);
            glVertex3f(GLpos[i].x, GLpos[i].y, GLpos[i].z);
            glVertex3f(GLpos[i+1].x, GLpos[i+1].y, GLpos[i+1].z);
        }
        glEnd();*/
        ///绘制正方体
        //drawBox(GLpos[GLpos.size()-1],0.02);
        drawBox(Mpos,1);
        //drawVec3(Mpos.acci);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLcamera.printString();
    cameraprint(Mpos,-0.9f,-0.9f);

    glutSwapBuffers();
}
void resize(GLint width, GLint height)
{
    if (height == 0)
        height = 1;
    float ratio = 1.0 * width / height;

    windowWidth = width;
    windowHeight = height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(45, ratio, 0.1, 1000);
    glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(cameraProjectionMatrix));
}
int GLmain(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(300, 50);
    glutInitWindowSize(800, 600);
    glutCreateWindow("MIMU MOUSE");
    glutDisplayFunc(&renderScene);
    glutIdleFunc(&renderScene);
    glutReshapeFunc(&resize);
    glutKeyboardFunc(&processNormalKeys);
    glutKeyboardUpFunc(&releaseNormalKey);
    glutSpecialFunc(&processSpecialKeys);
    glutSpecialUpFunc(&releaseSpecialKey);
    glutMouseFunc(&processMouse);
    glutMotionFunc(&processMouseActiveMotion);
    glutMouseWheelFunc(&processMouseWheel);
    glutPassiveMotionFunc(&processMousePassiveMotion);
    initGL();
    glutMainLoop();

    return false;
}
