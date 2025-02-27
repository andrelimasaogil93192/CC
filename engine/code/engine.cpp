#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "tinyxml2/tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class Point
{
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

class Vector
{
public:
	float x;
	float y;
	float z;

	Vector() {};
	Vector(float xCoord, float yCoord, float zCoord)
	{
		x = xCoord;
		y = yCoord;
		z = zCoord;
	}
};

class Projection
{
public:
	int fov;
	int near;
	int far;

	Projection() {};
	Projection(int newFov, int newNear, int newFar)
	{
		fov = newFov;
		near = newNear;
		far = newFar;
	}
};

class Window
{
public:
	int width;
	int height;

	Window() {};
	Window(int newWidth, int newHeight)
	{
		width = newWidth;
		height = newHeight;
	}
};

class Camera
{
public:
	Point position;
	Point lookAt;
	Vector upVector;
	Projection projection;

	Camera() {};
	Camera(Point newPosition, Point newLookAt, Vector newUpVector, Projection newProjection)
	{
		position = newPosition;
		lookAt = newLookAt;
		upVector = newUpVector;
		projection = newProjection;
	}
};

class Group
{
public:
	vector<string> models;
};

class World
{
public:
	Window window;
	Camera camera;
	vector <Group> groups;
};

// Global Variables

vector<Point> vertices;
World world;

int polygonMode = 0;

void loadXML(char* fileName)
{
	XMLDocument xmlFile;

	// Load the XML file into the Doc instance
	xmlFile.LoadFile(fileName);

	// Get root Element
	XMLElement* pRootElement = xmlFile.RootElement();

	// Enter rootElement which is World
	if (pRootElement != NULL)
	{
		// Enter window element
		XMLElement* pWindow = pRootElement->FirstChildElement("window");
		if (pWindow != NULL)
		{
			int width = stoi(pWindow->Attribute("width"));
			int height = stoi(pWindow->Attribute("height"));

			// Create window in global variable world
			world.window = Window(width, height);
		}

		// Enter camera element
		XMLElement* pCamera = pRootElement->FirstChildElement("camera");
		if (pCamera != NULL)
		{
			float x, y, z;
			Point position, lookAt;
			Vector upVector;
			Projection projection;

			// Enter position element
			XMLElement* pPosition = pCamera->FirstChildElement("position");
			if (pPosition != NULL)
			{
				// Camera Position
				x = stof(pPosition->Attribute("x"));
				y = stof(pPosition->Attribute("y"));
				z = stof(pPosition->Attribute("z"));

				position = Point(x, y, z);
			}

			// Enter lookAt element
			XMLElement* pLookAt = pCamera->FirstChildElement("lookAt");
			if (pLookAt != NULL)
			{
				// Camera lookAt
				x = stof(pLookAt->Attribute("x"));
				y = stof(pLookAt->Attribute("y"));
				z = stof(pLookAt->Attribute("z"));
				lookAt = Point(x, y, z);
			}

			// Enter up element
			XMLElement* pUpVector = pCamera->FirstChildElement("up");
			if (pUpVector != NULL)
			{
				// Camera upVector
				x = stof(pUpVector->Attribute("x"));
				y = stof(pUpVector->Attribute("y"));
				z = stof(pUpVector->Attribute("z"));
				upVector = Vector(x, y, z);
			}

			// Enter projection element
			XMLElement* pProjection = pCamera->FirstChildElement("projection");
			if (pProjection != NULL)
			{
				// Camera Projection
				x = stoi(pProjection->Attribute("fov"));
				y = stoi(pProjection->Attribute("near"));
				z = stoi(pProjection->Attribute("far"));
				projection = Projection(x, y, z);
			}

			// Create camera in global variable world
			world.camera = Camera(position, lookAt, upVector, projection);
		}

		// Run through every group element
		XMLElement* pGroup = pRootElement->FirstChildElement("group");
		while (pGroup)
		{
			Group group;

			// Enter models element
			XMLElement* pModels = pGroup->FirstChildElement("models");
			if (pModels)
			{

				// Run through every model element
				XMLElement* pModel = pModels->FirstChildElement("model");
				while (pModel)
				{

					// Add model to models vector in group
					group.models.push_back(pModel->Attribute("file"));

					// Change pointer to next model element
					pModel = pModel->NextSiblingElement("model");
				}
			}

			// Add model to group vector in world
			world.groups.push_back(group);

			// Change pointer to next group element
			pGroup = pGroup->NextSiblingElement("group");
		}
	}
}

void loadModels()
{
	for each (Group g in world.groups)
	{
		for each (string fileName in g.models)
		{
			ifstream file_pointer(fileName, ios::binary | ios::in);

			string linha;
			float x, y, z;

			while (getline(file_pointer, linha, '\0'))
			{
				sscanf_s(linha.c_str(), "%f %f %f", &x, &y, &z);
				Point p(x, y, z);
				vertices.push_back(p);
			}
			file_pointer.close();
		}
	}
}

void drawAxis()
{
	glBegin(GL_LINES);

	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);

	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
}

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you can’t make a window with zero width).
	if (h == 0)
		h = 1;
	// compute window's aspect ratio
	float ratio = w * 1.0f / h;
	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Set the perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void renderScene(void)
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set camera
	glLoadIdentity();
	gluLookAt(	world.camera.position.x,	world.camera.position.y,	world.camera.position.z,
				world.camera.lookAt.x,		world.camera.lookAt.y,		world.camera.lookAt.z,
				world.camera.upVector.x,	world.camera.upVector.y,	world.camera.upVector.z);

	// Aspect currently at 0.0f
	gluPerspective(world.camera.projection.fov, 0.0f, world.camera.projection.near, world.camera.projection.far);

	// put drawing instructions here
	drawAxis();

	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_TRIANGLES);
	for (Point p : vertices)
	{
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();


	// End of frame
	glutSwapBuffers();
}

void regular_keys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'p':
		if (polygonMode == 0) {
			polygonMode = 1;
			glPolygonMode(GL_FRONT, GL_LINE);
		}
		else {
			polygonMode = 0;
			glPolygonMode(GL_FRONT, GL_FILL);
		}
		break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "Insuficient Arguments, specify the xml file!";
		return 1;
	}
	else
	{
		loadXML(argv[1]);
	}

	loadModels();

	// put GLUT’s init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(world.window.width, world.window.height);
	glutCreateWindow("CG@DI");

	// put callback registry here
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);

	// Keyboard Functions
	glutKeyboardFunc(regular_keys);

	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// enter GLUT’s main cycle
	glutMainLoop();

	return 1;
}