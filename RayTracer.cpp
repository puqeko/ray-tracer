/*========================================================================
* COSC 363  Computer Graphics (2018)
* Ray tracer 
* See Lab07.pdf for details.
*=========================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "Sphere.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Cube.h"
#include "SceneObject.h"
#include "Ray.h"
using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 300;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;  //A global list containing pointers to objects in the scene


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);
	glm::vec3 light(10, 40, -3);
	glm::vec3 ambientCol(0.2);

    ray.closestPt(sceneObjects);

    if(ray.xindex == -1) return backgroundCol; //If there is no intersection return background colour

    glm::vec3 materialCol = sceneObjects[ray.xindex]->getColor();
	glm::vec3 colorSum = ambientCol * materialCol;  // default ambient color

	// diffuse
	glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);
	glm::vec3 lightVector = glm::normalize(light - ray.xpt);
	float lDotn = glm::dot(normalVector, lightVector);
	if (lDotn > 0) {
		// not in shadow
		Ray shadow(ray.xpt, lightVector);
		shadow.closestPt(sceneObjects);
		if (shadow.xindex == -1 || shadow.xdist > glm::length(light - ray.xpt)) {
			// not shadowed by another object
			colorSum += materialCol * lDotn;

			// specular color
			// TODO: fix this
			glm::vec3 reflVector = glm::reflect(-lightVector, normalVector);
			float rDotv = glm::dot(reflVector, -ray.dir);
			if (rDotv > 0) {
				colorSum += glm::vec3(1.0f) * pow(rDotv, 10.0f);
			}
		}

		// reflections
		bool isReflective = ray.xindex == 0;
		if (isReflective && step < MAX_STEPS) {
			glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
			Ray reflectedRay(ray.xpt, reflectedDir);
			glm::vec3 reflectedCol = trace(reflectedRay, step + 1);
			colorSum = colorSum + (0.8f * reflectedCol);
		}
	}

	return colorSum;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height
	float aniX = cellX / 4, aniY = cellY / 4;

	glm::vec3 eye(0., 0., 0.);  //The eye position (source of primary rays) is the origin

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a quad.

	for(int i = 0; i < NUMDIV; i++)  	//For each grid point xp, yp
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;

			glm::vec3 col = glm::vec3(0);
			float centerx = xp+0.5*cellX;
			float centery = yp+0.5*cellY;
			for (int l = -1; l <= 1; l += 2) {
				for (int r = -1; r <= 1; r += 2) {
					glm::vec3 dir(centerx + l * aniX, centery + r * aniY, -EDIST);	//direction of the primary ray

					Ray ray = Ray(eye, dir);		//Create a ray originating from the camera in the direction 'dir'
					ray.normalize();				//Normalize the direction of the ray to a unit vector
					col += trace(ray, 1); //Trace the primary ray and get the colour value
				}
			}

			col /= 4;  // average of 4 cells
			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}


//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);

	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, 0.0, -90.0), 15.0, glm::vec3(0, 0, 1));
	Cylinder *cylinder = new Cylinder(glm::vec3(5.0, -15.0, -70.0), 4.0, glm::vec3(1, 0, 0));
	// Sphere *sphere2 = new Sphere(glm::vec3(5.0, 5.0, -70.0), 4.0, glm::vec3(1, 0, 0));
	// Sphere *sphere3 = new Sphere(glm::vec3(3.0, -15.0, -70.0), 4.0, glm::vec3(0, 1, 0));
	Plane *plane = new Plane (glm::vec3(-20., -20, -40),
                              glm::vec3(20., -20, -40),
                              glm::vec3(20., -20, -200),
                              glm::vec3(-20., -20, -200),
                              glm::vec3(0.5, 0.5, 0),
							  true);  // infinate plane
	// Cube *cube = new Cube(5, glm::vec3(-10, -10, -80), glm::vec3(0.5, 0.5, 0));

	sceneObjects.push_back(sphere1); 
	// sceneObjects.push_back(cube); 
	sceneObjects.push_back(cylinder);
	// sceneObjects.push_back(sphere3);
	sceneObjects.push_back(plane);
}


int main(int argc, char *argv[]) {
	cout << "Lab 8" << endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
