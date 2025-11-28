#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;


struct Color { float r, g, b; };

class Object
{
public:
    double xp, yp, zp, size;
    Color color;
    Object(double xp, double yp, double zp, double size, Color c)
    {
        this->xp = xp;
        this->yp = yp;
        this->zp = zp;
        this->size = size;
        this->color = c;
    }
};

class Teapot : public Object
{
public:
    Teapot(double xp, double yp, double zp, double size, Color c) : Object(xp, yp, zp, size, c)
    {};

    void make()
    {
        glPushMatrix();
        glTranslatef(this->xp, this->yp, this->zp);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glutSolidTeapot(this->size);
        glPopMatrix();
    }
};

class Cube : public Object
{
public:
    Cube(double xp, double yp, double zp, double size, Color c) : Object(xp, yp, zp, size, c) {};
    void make()
    {
        glPushMatrix();
        glTranslatef(this->xp, this->yp, this->zp);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glutSolidCube(this->size);
        glPopMatrix();
    }
};

class Sphere : public Object
{
public:
    Sphere(double xp, double yp, double zp, double size, Color c) : Object(xp, yp, zp, size, c) {};
    void make()
    {
        glPushMatrix();
        glTranslatef(this->xp, this->yp, this->zp);
        glColor3f(this->color.r, this->color.g, this->color.b);
        glutSolidSphere(this->size, 20, 20);
        glPopMatrix();
    }
};

double camX = 0.0, camY = 0.0, camZ = 5.0;

double yaw = 0.0;
double pitch = 0.0;

int lastMouseX = 450;
int lastMouseY = 450;

bool firstMouse = true;

double speed = 0.05;

double gspeed = 0;

bool keys[256] = { false };

void computeCamera(double& dirX, double& dirY, double& dirZ)
{
    double radian_Yaw = yaw * 3.1415 / 180.0;
    double radian_Pitch = pitch * 3.1415 / 180.0;

    dirX = cos(radian_Pitch) * cos(radian_Yaw);
    dirY = sin(radian_Pitch);
    dirZ = cos(radian_Pitch) * sin(radian_Yaw);
}

void moveCamera()
{
    //int isShift = glutGetModifiers();

    double dirX, dirY, dirZ;
    computeCamera(dirX, dirY, dirZ);

    double forwardX = dirX;
    double forwardZ = dirZ;


    double rightX = dirZ;
    double rightZ = -dirX;

    if (keys['w'])
    {
        camX += forwardX * speed;
        camZ += forwardZ * speed;
    }

    if (keys['s'])
    {
        camX -= forwardX * speed;
        camZ -= forwardZ * speed;
    }

    if (keys['a'])
    {
        camX += rightX * speed;
        camZ += rightZ * speed;
    }

    if (keys['d'])
    {
        camX -= rightX * speed;
        camZ -= rightZ * speed;
    }

    if (keys[32]) camY = 5;
    if (keys[27]) exit(0);
    //if (GetAsyncKeyState(VK_SHIFT) & 0x8000) speed = 0.2;
    //else speed = 0.05;

}

void gravity()
{
    gspeed += 0.0005;
    if (camY > 0)
    {
        camY -= gspeed;
    }
    else if (camY < 0)
    {
        camY = 0;
        gspeed = 0;
    };
}

void add_objects()
{
    Teapot x = Teapot(10, 0, 10, 3, { 1,1,0 });
    x.make();

    Cube c = Cube(50, 0, -20, 5, { 0,0,1 });
    c.make();

    Sphere s = Sphere(100, 0, -50, 4, { 0,1,0 });
    s.make();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    double dirX, dirY, dirZ;
    computeCamera(dirX, dirY, dirZ);

    gluLookAt(
        camX, camY, camZ,
        camX + dirX, camY + dirY, camZ + dirZ,
        0, 1, 0
    );

    moveCamera();

    add_objects();
    gravity();
    glutSwapBuffers();

}


void mouseMotion(int x, int y)
{
    int dx = x - (lastMouseX);
    int dy = y - (lastMouseY);

    double sensitivity = 0.15;


    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;

    glutWarpPointer(lastMouseX, lastMouseY);

    if (pitch > 90.0) pitch = 90.0;
    if (pitch < -90.0) pitch = -90.0;

    if (yaw > 360) yaw -= 360;
    if (yaw < 0) yaw += 360;


    glutPostRedisplay();
}

void KeyboardDownf(unsigned char key, int xx, int yy)
{
    keys[key] = true;
}

void KeyboardUpf(unsigned char key, int xx, int yy)
{
    keys[key] = false;
}

void lightshading()
{
    GLfloat light_position[] = { 5, 5, 5, 1 };
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}

void init()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.8, 0.8, 0.8, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 0.1, 100);

    glMatrixMode(GL_MODELVIEW);

    glutWarpPointer(lastMouseX, lastMouseY);
    glutSetCursor(GLUT_CURSOR_NONE);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(360, 30);
    glutCreateWindow("MyProject ver 1.1");

    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(KeyboardDownf);
    glutKeyboardUpFunc(KeyboardUpf);

    init();
    lightshading();

    glutMainLoop();
}
