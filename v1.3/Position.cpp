#include "Position.h"
#include <iostream>
using namespace std;

Position::Position(double x1, double x2, double y1, double y2, double z1, double z2)
{
	this->Xmin = x1;
	this->Xmax = x2;
	this->Ymin = y1;
	this->Ymax = y2;
	this->Zmin = z1;
	this->Zmax = z2;
}

Position::Position()
{
	Xmin = Xmax = Ymax = Ymin = Zmax = Zmin = 0;
}

void Position::writeDim()
{
	cout << "----------------------------\n";
	cout << "X = (" << this->Xmin << " to " << this->Xmax << ")\n";
	cout << "Y = (" << this->Ymin << " to " << this->Ymax << ")\n";
	cout << "Z = (" << this->Zmin << " to " << this->Zmax << ")\n";
	cout << "----------------------------\n";
}
