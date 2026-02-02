#include <cmath>
#include "Color.h"
#include "Object.h"
#include "Teapot.h"
#include "Cube.h"
#include "Sphere.h"
#include "WallX.h"
#include "WallZ.h"
#include "Figure.h"
#include<iostream>
#include<vector>
#include "Bullet.h"

using namespace std;

double camX = 0.0, camY = 0.0, camZ = 5.0;

double yaw = 0.0;
double pitch = 0.0;

int lastMouseX = 450;
int lastMouseY = 450;

//bool firstMouse = true;

double speed = 0.01;

double gravity = 1;
double Yspeed = 0;

bool onAir = false;

double groundLevel = 0;
double cameraDistance = 3;

bool keys[256] = {false};

double t1 = 0,t2 = 0;

vector<Position> collider;
vector<Bullet> bullets;

Figure player(camX, 0, camZ, 0.3, { 1,1,0 });

bool firstPersonView = true;

void computeCamera(double& dirX, double& dirY, double& dirZ);
double getRealTime(double& t1, double& t2);
void jump(double upsv);
void moveCamera(double dt);
void gravityf(double dt);
void makeGround(double sx, double sz, double ex, double ez);
void add_objects(double dt);
void update(double dt);
void mouseMotion(int x, int y);
void KeyboardDownf(unsigned char key, int xx, int yy);
void KeyboardUpf(unsigned char key, int xx, int yy);
void make_screen_2D();
void make_screen_3D();
void writeText(float x, float y, const char* text);
void pauseMenu();
void mouseFunc(int button, int state, int x, int y);

void computeCamera(double& dirX, double& dirY, double& dirZ)
{
        double radian_Yaw = yaw * 3.1415 / 180.0;
        double radian_Pitch = pitch * 3.1415 / 180.0;

        dirX = cos(radian_Pitch) * cos(radian_Yaw);
        dirY = sin(radian_Pitch);
        dirZ = cos(radian_Pitch) * sin(radian_Yaw);

}

double getRealTime(double& t1, double& t2)
{
    t2 = glutGet(GLUT_ELAPSED_TIME);
    double delta = (t2 - t1);
    t1 = t2;
    return delta;
}

void jump(double upsv)
{
    Yspeed = upsv;
    onAir = true;
}

void moveCamera(double dt)
{
    double dirX, dirY, dirZ;
    computeCamera(dirX, dirY, dirZ);

    double forwardX = dirX;
    double forwardZ = dirZ;

    double rightX = forwardZ;
    double rightZ = -forwardX;

    double realSpeed = dt * speed;
    double moveX = 0;
    double moveZ = 0;

    static bool pressed = false;

    if (keys['w'])
    {
        moveX += forwardX;
        moveZ += forwardZ;
    }

    if (keys['s'])
    {
        moveX -= forwardX;
        moveZ -= forwardZ;
    }

    if (keys['a'])
    {
        moveX += rightX;
        moveZ += rightZ;
    }

    if (keys['d'])
    {
        moveX -= rightX;
        moveZ -= rightZ;
    }

    

    moveX *= realSpeed;
    moveZ *= realSpeed;

    bool goX = true;
    bool goZ = true;

    for (const Position& p : collider)
    {
        if (player.collides(player.xp + moveX, player.zp,player.yp, p)) goX = false;
        if (player.collides(player.xp, player.zp + moveZ,player.yp, p)) goZ = false;
        if ((player.xp < p.Xmax) && (player.xp > p.Xmin) && (player.zp < p.Zmax) && (player.zp > p.Zmin))
        {
            groundLevel = p.Ymax + (2 * player.size);
            break;
        }
        else
        {
            groundLevel = 0;
            break;
        }


    }

    if (goX) camX += moveX;
    if (goZ) camZ += moveZ;

  
    onAir = player.yp > groundLevel + 0.05;

    player.moveFigure(camX + 0.5, camY - groundLevel, camZ + 0.5,!firstPersonView);

    if (keys[32] && !onAir) jump(3);
    if (keys['v'] && !pressed)
    {
        firstPersonView = !firstPersonView;
        pressed = true;
    }
    if (!keys['v']) pressed = false;
    if (keys[27]) exit(0);  
}

void gravityf(double dt)
{
   // cout << groundLevel << "\t";
    if (onAir)
    {
        camY += Yspeed * dt * 0.01;
        Yspeed -= gravity * dt * 0.01;
        if (camY <= groundLevel)
        {
            camY = groundLevel;
            Yspeed = 0;
            onAir = false;
        }
    }
}

void makeGround(double sx, double sz, double ex, double ez)
{
    for (int i = 0; i < ex - sx; i++)
    {
        for (int j = 0; j < ez - sz; j++)
        {
            glPushMatrix();
            glTranslatef(i, -1.5, j);
            glColor3f(0, 0.75, 0);
            glutSolidCube(1);
            glPopMatrix();
        }
    }

}

void add_objects(double dt)
{

    Cube c = Cube(50, 0, -20, 2, { 0,0,1 });
    c.make();
    c.makeWireCube();
    if (!c.inList)
    {
        collider.push_back(c.getDim());
        c.inList = true;
    }

    Sphere s = Sphere(100, 0, -50, 4, { 0,1,0 });
    //s.moveObject = [&]()
    //{
    //    s.xp += 2.0  * cos(glutGet(GLUT_ELAPSED_TIME) * 0.005);
    //    Position p = s.getDim();
    //};
   // s.moveObject();
    s.make();
    s.makeWireCube();
    if (!s.inList)
    {
        collider.push_back(s.getDim());
        s.inList = true;
    }

    //WallX w = WallX(0, 20, 0, 5, { 1,0,0 });
    //w.moveObject = [&]()
    //{
       // w.startingX += cos(glutGet(GLUT_ELAPSED_TIME) * 0.001);
       // w.endingX += cos(glutGet(GLUT_ELAPSED_TIME) * 0.001) ;
    //};
    //w.moveObject();
    //w.make();

    WallZ wx = WallZ(-20, 20, -20, 3, { 1,0.25,0 });
    wx.make();

    makeGround(-10, -10, 100, 100);

}

void update(double dt)
{
    moveCamera(dt);
    add_objects(dt);
    gravityf(dt);

    for (auto it = bullets.begin(); it != bullets.end(); )
    {
        if (it->render)
        {
            it->makeShape(dt);
            ++it;
        }
        else
        {
            it = bullets.erase(it);
        }
    }

}
void make_screen_2D()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 900, 0, 900);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
}

void make_screen_3D()
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void writeText(float x, float y, const char* text)
{
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void pauseMenu()
{
    make_screen_2D();
    glColor3f(1, 1, 1);
    writeText(400, 700, "My game");

    glColor3f(0, 1, 0);
    writeText(320, 300, "1-Start game");

    glColor3f(0, 0, 1);
    writeText(600, 300, "2-quit");
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    double dirX, dirY, dirZ;

    computeCamera(dirX, dirY, dirZ);

    if (firstPersonView) 
    {
        gluLookAt(
            camX,camY,camZ,
            camX + dirX, camY + dirY, camZ + dirZ,
            0, 1, 0
        );
    }
    else
    {
        gluLookAt(
            player.xp - (cameraDistance * dirX), player.yp + (cameraDistance * dirY), player.zp - (cameraDistance * dirZ),
            player.xp , player.yp , player.zp,
            0, 1, 0
        );
    }

     double dt = getRealTime(t1, t2);
     update(dt);
     glutPostRedisplay();
    glutSwapBuffers();
}

void mouseMotion(int x, int y)
{
    int dx = x - (lastMouseX);
    int dy = y - (lastMouseY);

    double sensitivity = 0.08;


    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;

    glutWarpPointer(lastMouseX, lastMouseY);

    if (pitch > 90.0) pitch = 90.0;
    if (pitch < -90.0) pitch = -90.0;

    if (yaw > 360) yaw -= 360;
    if (yaw < 0) yaw += 360;

    
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

    //glutWarpPointer(lastMouseX, lastMouseY);    delete!
    glutSetCursor(GLUT_CURSOR_NONE);
    glLoadIdentity();
}

void mouseFunc(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        double dirX, dirY, dirZ;
        computeCamera(dirX, dirY, dirZ);


        if (firstPersonView)
        {
            bullets.push_back(Bullet(
                Position(camX - player.size, camX + player.size, camY - player.size, camY + player.size, camZ - player.size, camZ + player.size),
                { 1,0,0 },
                dirX,
                dirY,
                dirZ));
        }
        else
        {
            bullets.push_back(Bullet(
                Position(player.xp, player.xp - player.size - 1, player.yp, player.yp + player.size, player.zp, player.zp - player.size),
                { 1,0,0 },
                dirX,
                -dirY,
                dirZ
            ));
        }
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(360, 30);
    glutCreateWindow("MyProject ver 1.3");

    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(KeyboardDownf);
    glutKeyboardUpFunc(KeyboardUpf);
    glutMouseFunc(mouseFunc);

    init();
    lightshading();

    glutMainLoop();
}
