#define _USE_MATH_DEFINES
#define M_PI	3.14159265358979323846

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

class Point {
public:
	float x;
	float y;
	float z;

	Point() {};
	Point(float xCoord, float yCoord, float zCoord)
	{
		x = xCoord;
		y = yCoord;
		z = zCoord;
	}

	string toString()
	{
		return to_string(x) + " " + to_string(y) + " " + to_string(z);
	}
};

void writePoint(Point p, ofstream& file)
{
	string point = p.toString();
	file.write(point.c_str(), point.length() + 1);
}

void writeSquare(Point p1, Point p2, Point p3, Point p4, ofstream& file)
{
	/*
	* Writes the points of the two triangles that form a square which points
	* are given in the same order as we read a book (left-right, top-bottom)
	*	 p1		 p2
	*	  ___
	*	 |  /  /|
	*	 | /  / |
	*	 |/  /__|
	*	 p3     p4
	*/

	// First triangle
	writePoint(p1, file);
	writePoint(p3, file);
	writePoint(p2, file);

	// Second triangle
	writePoint(p3, file);
	writePoint(p4, file);
	writePoint(p2, file);
}


void plane(int length, int division, char* fileName)
{
	ofstream file(fileName, ios::binary | ios::out);

	/*
	* Draw strategy:
	* The idea is to draw a 2D square using
	* the points estimated as follows:
	*       X
	*       ^
	* p1----|----p2
	* |     |    |
	* |     -----|--> Z
	* |          |
	* p3---------p4
	*
	* Since the plane is always centered on the origin, its vertices
	* are located at half the side length from the origin.
	*/

	float v = (float)length / 2; // value of the coordinate for all the corner vertices

	Point p1 = {  v, 0, -v };
	Point p2 = {  v, 0,  v };
	Point p3 = { -v, 0, -v };
	Point p4 = { -v, 0,  v };

	float inc = (float)length / division; // increment for internal vertices

	Point x1, x2, x3, x4; // temp points to use when building sub-faces

	for (int i = 0; i < division; i++)
	{
		for (int j = 0; j < division; j++)
		{
			/*
			* x1-----x2
			* |      |
			* |      |
			* x3-----x4
			* p3
			*/
			x1 = { p3.x + inc * (1 + i), p3.y, p3.z + inc * j       };
			x2 = { p3.x + inc * (1 + i), p3.y, p3.z + inc * (1 + j) };
			x3 = { p3.x + inc * i      , p3.y, p3.z + inc * j       };
			x4 = { p3.x + inc * i      , p3.y, p3.z + inc * (1 + j) };

			writeSquare(x1, x2, x3, x4, file);
		}
	}

	file.close();
}


void box(int length, int division, char* fileName)
{
	ofstream file(fileName, ios::binary | ios::out);

	/*
	* Draw strategy:
	* The idea is to draw 2D squares for each side of the box using
	* the corresponding points estimated as follows:
	*     p5-------p6
	*   /  |       /|
	*  /   |      / |
	* p1---|-----p2 |
	* |    |     |  |
	* |   p7-----|-p8
	* | /        | /
	* p3---------p4
	*
	* Since the box is always centered on the origin, its vertices
	* are located at half the side length from the origin.
	*/

	float v = (float)length / 2; // value of the coordinate for all the box corner vertices
	
	Point p1 = {-v,  v,  v};
	Point p2 = { v,  v,  v};
	Point p3 = {-v, -v,  v};
	Point p4 = { v, -v,  v};
	Point p5 = {-v,  v, -v};
	Point p6 = { v,  v, -v};
	Point p7 = {-v, -v, -v};
	Point p8 = { v, -v, -v};

	float inc = (float)length / division; // increment for internal vertices

	Point x1, x2, x3, x4; // temp points to use when building faces

	// Top Face
	for (int i = 0; i < division; i++)
	{
		for (int j = 0; j < division; j++)
		{
			/*
			* x1-----x2
			* |      |
			* |      |
			* x3-----x4
			* p6
			*/
			x1 = { p6.x - inc * i      , p6.y, p6.z + inc * (1 + j) };
			x2 = { p6.x - inc * (1 + i), p6.y, p6.z + inc * (1 + j) };
			x3 = { p6.x - inc * i      , p6.y, p6.z + inc * j       };
			x4 = { p6.x - inc * (1 + i), p6.y, p6.z + inc * j       };

			writeSquare(x1, x2, x3, x4, file);
		}
	}

	// Down Face
	for (int i = 0; i < division; i++)
	{
		for (int j = 0; j < division; j++)
		{
			/*
			* x1-----x2
			* |      |
			* |      |
			* x3-----x4
			* p7
			*/
			x1 = { p7.x + inc * i      , p7.y, p7.z + inc * (1 + j) };
			x2 = { p7.x + inc * (1 + i), p7.y, p7.z + inc * (1 + j) };
			x3 = { p7.x + inc * i      , p7.y, p7.z + inc * j       };
			x4 = { p7.x + inc * (1 + i), p7.y, p7.z + inc * j       };

			writeSquare(x1, x2, x3, x4, file);
		}
	}

	// Front Face
	for (int i = 0; i < division; i++)
	{
		for (int j = 0; j < division; j++)
		{
			/*
			* x1-----x2
			* |      |
			* |      |
			* x3-----x4
			* p3
			*/
			x1 = { p3.x + inc * i      , p3.y + inc * (1 + j) , p3.z };
			x2 = { p3.x + inc * (1 + i), p3.y + inc * (1 + j) , p3.z };
			x3 = { p3.x + inc * i      , p3.y + inc * j       , p3.z };
			x4 = { p3.x + inc * (1 + i), p3.y + inc * j       , p3.z };

			writeSquare(x1, x2, x3, x4, file);
		}
	}

	// Rear Face
	for (int i = 0; i < division; i++)
	{
		for (int j = 0; j < division; j++)
		{
			/*
			* x1-----x2
			* |      |
			* |      |
			* x3-----x4
			* p8
			*/
			x1 = { p8.x - inc * i      , p8.y + inc * (1 + j) , p8.z };
			x2 = { p8.x - inc * (1 + i), p8.y + inc * (1 + j) , p8.z };
			x3 = { p8.x - inc * i      , p8.y + inc * j       , p8.z };
			x4 = { p8.x - inc * (1 + i), p8.y + inc * j       , p8.z };

			writeSquare(x1, x2, x3, x4, file);
		}
	}

	// Right Face
	for (int i = 0; i < division; i++)
	{
		for (int j = 0; j < division; j++)
		{
			/*
			* x1-----x2
			* |      |
			* |      |
			* x3-----x4
			* p4
			*/
			x1 = { p4.x, p4.y + inc * (1 + i), p4.z - inc * j };
			x2 = { p4.x, p4.y + inc * (1 + i), p4.z - inc * (1 + j) };
			x3 = { p4.x, p4.y + inc * i      , p4.z - inc * j       };
			x4 = { p4.x, p4.y + inc * i      , p4.z - inc * (1 + j) };

			writeSquare(x1, x2, x3, x4, file);
		}
	}

	// Leftface
	for (int i = 0; i < division; i++)
	{
		for (int j = 0; j < division; j++)
		{
			/*
			* x1-----x2
			* |      |
			* |      |
			* x3-----x4
			* p7
			*/
			x1 = { p7.x, p7.y + inc * (1 + i), p7.z + inc * j };
			x2 = { p7.x, p7.y + inc * (1 + i), p7.z + inc * (1 + j) };
			x3 = { p7.x, p7.y + inc * i      , p7.z + inc * j };
			x4 = { p7.x, p7.y + inc * i      , p7.z + inc * (1 + j) };

			writeSquare(x1, x2, x3, x4, file);
		}
	}

	file.close();
}

void sphere(float radius, int slices, int stacks, char* fileName)
{
	// Horizontal Circle
	float alpha = 0;
	float alpha_inc = (float)(2 * M_PI) / (float)slices;
	// Half Vertical Circle
	float beta = (float)-M_PI / 2;
	float beta_inc = (float)(M_PI) / (float)stacks;

	Point p1, p2, p3, p4;

	ofstream file(fileName, ios::binary | ios::out);

	for (int i = 1; i < slices + 1; i++) {
		for (int j = 1; j < stacks + 1; j++) {

			p1 = { radius * cos(beta) * sin(alpha),							radius * sin(beta),				radius * cos(beta) * cos(alpha) };
			p2 = { radius * cos(beta + beta_inc) * sin(alpha),				radius * sin(beta + beta_inc),	radius * cos(beta + beta_inc) * cos(alpha) };
			p3 = { radius * cos(beta + beta_inc) * sin(alpha + alpha_inc),	radius * sin(beta + beta_inc),	radius * cos(beta + beta_inc) * cos(alpha + alpha_inc) };
			p4 = { radius * cos(beta) * sin(alpha + alpha_inc),				radius * sin(beta),				radius * cos(beta) * cos(alpha + alpha_inc) };

			/*
			*	p2		p3
			*	  ______
			*	  \    /
			*	   \  /
			*	    \/
			*	 p4 = p1
			*   First stack (under) is a triangle
			*/
			if (j == 1) {
				writePoint(p3, file);
				writePoint(p2, file);
				writePoint(p1, file);
			}

			/*
			*	 p2 = p3
			*		/\
			*	   /  \
			*	  /____\
			*	p1		p4
			*	Last stack (top) is a triangle
			*/
			else if (j == stacks) {
				writePoint(p4, file);
				writePoint(p3, file);
				writePoint(p1, file);
			}

			/*
			*	Sentido do Relógio
			*	2 +-------+ 3
			*	  |		 /|
			*	  |	  /	  |
			*	  |/	  |
			*	1 +-------+ 4
			*/
			else {
				writePoint(p4, file);
				writePoint(p3, file);
				writePoint(p2, file);

				writePoint(p2, file);
				writePoint(p1, file);
				writePoint(p4, file);
			}
			beta = (float)-(M_PI / 2) + j * beta_inc;
		}
		alpha = i * alpha_inc;
		beta = (float)-(M_PI / 2);
	}
	file.close();
}

void cone(float radius, float height, int slices, int stacks, char* filename) {
	/*
	* Draw strategy:
	* The idea is to draw 2D circles for different yy positions (corresponding to
	* the different stacks). Starting from the bottom of the cone (where the circle
	* should also be drawn), in each iteration we'll be drawing 2 points in the current
	* circle and 2 points in the upper circle. Those will define the side of the cone.
	* In the last stack, the upper circle doesn't exist and, instead of 2 points on the
	* upper side, there will only exist 1 - the top vertice of the cone.
	*/
	float alpha = 0; // initial angle to travel through the base perimeter
	float alpha_inc = (float)(2 * M_PI) / (float)slices; // angle increment to be used in each iteration
	float h = 0; // initial value of current height
	float h_inc = (float)height / (float)stacks; // increment of height 
	float r = radius; // initial value of current radius
	float new_r; // radius of the circle on top of the current one

	Point p1, p2, p3, p4;

	ofstream file(filename, ios::binary | ios::out);

	for (int i = 1; i < slices + 1; i++) {
		/*
		* Triangle in the base (YY coordinate equals to zero)
		*          x p3 (center of circle)
		*       /   /
		*      /    /
		* p1 /     /
		*  x      /
		*    \   /
		*     \ /
		*      x  p2
		*/
		p1 = { radius * sin(alpha),             0, radius * cos(alpha) };
		p2 = { radius * sin(alpha + alpha_inc), 0, radius * cos(alpha + alpha_inc) };
		p3 = { 0,                               0, 0 };

		writePoint(p1, file);
		writePoint(p3, file);
		writePoint(p2, file);

		for (int j = 1; j < stacks + 1; j++) {

			/*
			* To identify the radius of the circle on top of the current one, just need to consider the
			* triangle that can be draw be cutting the cone in half (either through YZ or XY plane).
			* Then, just compare the triangle with total cone helgth as vertical side with that having
			* a vertical side that goes from bottom of cone up to next stack level.
			* 
			* 			              From the geometry on the left one gets:
			*          /|             radius ----- heigth
			*         / |                r   ----- heigth - (h + h_inc)
			*        /  |
			*       /   |             Thus r = radius * (height - (h + h_inc)) / height
			*      /____| height
			*     /  r  |
			*    /      |
			*   /       |
			*  ----------
			*     radius
			*/

			new_r = (radius * (height - (h + h_inc))) / height;

			/* p3 and p4 are in the current level (YY equal), p1 and p2 are on the upper level(YY equal)
			*  p1       p2
			*  x--------x
			*  |        |
			*  |        |
			*  |        |
			*  |        |
			*  x--------x
			*  p3       p4
			*/
			p1 = { new_r * sin(alpha)            , h + h_inc, new_r * cos(alpha) };
			p2 = { new_r * sin(alpha + alpha_inc), h + h_inc, new_r * cos(alpha + alpha_inc) };
			p3 = { r     * sin(alpha)            ,     h    , r     * cos(alpha) };
			p4 = { r     * sin(alpha + alpha_inc),     h    , r     * cos(alpha + alpha_inc) };

			writeSquare(p1, p2, p3, p4, file);

			h = j * h_inc;
			r = new_r;
		}
		alpha = i * alpha_inc;
		h = 0;
		r = radius;
	}
	file.close();
}

void cylinder(float radius, float height, int slices, char* filename) {
	/*
	* Draw strategy:
	* Since the cylinder will be centered in the origin of its own axis
	* we'll draw half of it and then multiply by -1 to get the other half.
	* To do this, we'll loop through cylinder basis angle and, while describing
	* the circle, draw one slice of the cylinder at a time.
	* 
	*  p1       p2
	*  x--------x
	*  |        |
	*  |        |
	*  |        |
	*  |        |
	*  x--------x
	*  p3       p4
	*/
	float alpha = 0; // initial angle to travel through the base perimeter
	float alpha_inc = (float)(2 * M_PI) / (float)slices; // angle increment to be used in each iteration
	float halfHeight = (float)height / 2;

	Point topBaseCenter = { 0, halfHeight, 0 };
	Point botBaseCenter = { 0, -1 * halfHeight, 0 };

	Point p1, p2, p3, p4;

	ofstream file(filename, ios::binary | ios::out);

	for (int i = 1; i < slices + 1; i++) {
		// note that, since we assume that cylinder basis is parallel to XZ plane,
		// YY coordinate will remain constant in all points (apart from the simmetry)
		p1 = { radius * (float)sin(alpha),                  halfHeight, radius * (float)cos(alpha) };
		p2 = { radius * (float)sin(alpha + alpha_inc),      halfHeight, radius * (float)cos(alpha + alpha_inc) };
		p3 = { radius * (float)sin(alpha),             -1 * halfHeight, radius * (float)cos(alpha) };
		p4 = { radius * (float)sin(alpha + alpha_inc), -1 * halfHeight, radius * (float)cos(alpha + alpha_inc) };

		// triangle on top base
		writePoint(topBaseCenter, file);
		writePoint(p1, file);
		writePoint(p2, file);

		// square on lateral side
		writeSquare(p1, p2, p3, p4, file);

		// triangle on bottom base
		writePoint(botBaseCenter, file);
		writePoint(p4, file);
		writePoint(p3, file);

		alpha += alpha_inc;
	}
	file.close();
}

void generatePrimitive(int argc, char* argv[], int primitive)
{
	switch (primitive)
	{
	case 1:
		if (argc < 5)
		{
			std::cout << "Insuficient arguments for plane, requires 4!" << std::endl;
		}
		else
		{
			int length = stoi(argv[2]);
			int division = stoi(argv[3]);
			char* fileName = argv[4];
			plane(length, division, fileName);
		}
		break;

	case 2:
		if (argc < 5)
		{
			std::cout << "Insuficient arguments for box, requires 4!" << std::endl;
		}
		else
		{
			int length = stoi(argv[2]);
			int division = stoi(argv[3]);
			char* fileName = argv[4];
			box(length, division, fileName);
		}
		break;

	case 3:
		if (argc < 6)
		{
			std::cout << "Insuficient arguments for sphere, requires 5!" << std::endl;
		}
		else
		{
			float radius = stof(argv[2]);
			int slices = stoi(argv[3]);
			int stacks = stoi(argv[4]);
			char* fileName = argv[5];
			sphere(radius, slices, stacks, fileName);
		}
		break;

	case 4:
		if (argc < 7)
		{
			std::cout << "Insuficient arguments for cone, requires 6!" << std::endl;
		}
		else
		{
			float radius = stof(argv[2]);
			float height = stof(argv[3]);
			int slices = stoi(argv[4]);
			int stacks = stoi(argv[5]);
			char* fileName = argv[6];
			cone(radius, height, slices, stacks, fileName);
		}
		break;

	case 5:
		if (argc < 5)
		{
			std::cout << "Insuficient arguments for cylinder, requires 4!" << std::endl;
		}
		else
		{
			float radius = stof(argv[2]);
			float height = stof(argv[3]);
			int slices = stoi(argv[4]);
			char* fileName = argv[5];
			cylinder(radius, height, slices, fileName);
		}
		break;
	}
}

int main(int argc, char** argv)
{
	if (argc <= 4) {
		std::cout << "Insuficient arguments, requires at least 4!" << std::endl;
	}
	else {
		int primitiveCode = 0;
		string primitive = argv[1];

		if (primitive == "plane")	primitiveCode = 1;
		else if (primitive == "box")	primitiveCode = 2;
		else if (primitive == "sphere")	primitiveCode = 3;
		else if (primitive == "cone")	primitiveCode = 4;
		else if (primitive == "cylinder") primitiveCode = 5;
		else std::cout << "Primitive non existent!" << std::endl;

		generatePrimitive(argc, argv, primitiveCode);
	}

	return 1;
}