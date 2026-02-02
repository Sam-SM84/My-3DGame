#include "Color.h"
#include "Position.h"
#include "Figure.h"
#include<GL/glut.h>

Figure::Figure(double x,double y,double z,double s, Color c)
{
	this->xp = x;
	this->yp = y;
	this->zp = z;
	this->color = c;
	this->size = s;
}

bool between(double x, double a, double b)
{
	return (x >= a && x <= b);
}

bool Figure::collides(double nx, double nz,double ny, Position p)
{
	return (between(nx, p.Xmin - 0.1, p.Xmax + 0.1) && between(nz, p.Zmin - 0.1, p.Zmax + 0.1) && (ny <= p.Ymax));
}

void Figure::moveFigure(double x,double y,double z,bool fs)
{
	this->xp = x;
	this->yp = y;
	this->zp = z;
	if (fs)
	{
		glPushMatrix();
		glTranslatef(this->xp, this->yp, this->zp);
		glColor3f(this->color.r, this->color.g, this->color.b);
		glutSolidCube(this->size);
		glPopMatrix();
	}
}



