/*========================================================================
* COSC 363  Computer Graphics (2018)
* Ray tracer 
* See Lab07.pdf for details.
*=========================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "Sphere.h"
#include "SphereTex.h"
#include "Torus.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Cube.h"
#include "SceneObject.h"
#include "Ray.h"
using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int ANTIALIAS_LEVEL = 4;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;
char moonFilename[] = "../resources/moon.bmp";

#define PI 3.14159265358979f

vector<SceneObject*> sceneObjects;  //A global list containing pointers to objects in the scene

//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0.8);
	glm::vec3 light(10, 40, -3);
	glm::vec3 ambientCol(0.2);

    ray.closestPt(sceneObjects);

    if (ray.xindex == -1) return backgroundCol; //If there is no intersection return background colour

	// if (ray.xindex)
    glm::vec3 materialCol = sceneObjects[ray.xindex]->getColor(ray.xpt);
	glm::vec3 colorSum = ambientCol * materialCol;  // default ambient color

	// diffuse
	glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);
	glm::vec3 lightVector = glm::normalize(light - ray.xpt);
	float lDotn = glm::dot(normalVector, lightVector);

		// transparancy
	if (sceneObjects[ray.xindex]->opacity < 1.0 && step < MAX_STEPS) {

		glm::vec3 g;
		if (glm::dot(normalVector, ray.dir) < 0) {
			// if entering the object
			g = glm::refract(ray.dir, normalVector, sceneObjects[ray.xindex]->refractiveIndex);
		} else {
			// if exiting
			g = glm::refract(ray.dir, -normalVector, 1.0f/sceneObjects[ray.xindex]->refractiveIndex);
		}

		Ray throughRay(ray.xpt, g);
		glm::vec3 refractedCol = trace(throughRay, step + 1);
		float op = sceneObjects[ray.xindex]->opacity;
		colorSum = ((1.0f - op) * refractedCol) + (op * colorSum);
	}

	if (lDotn > 0) {
		// not in shadow
		Ray shadow(ray.xpt, lightVector);
		shadow.closestPt(sceneObjects);

		// diffuse
		// assuming not covered by another object
		glm::vec3 lighting = materialCol * lDotn * sceneObjects[ray.xindex]->opacity;

		// specular color
		glm::vec3 reflVector = glm::reflect(-lightVector, normalVector);
		float rDotv = glm::dot(reflVector, -ray.dir);
		if (rDotv > 0) {
			lighting += glm::vec3(1) * pow(rDotv, 10.0f);
		}

		if (shadow.xindex == -1 || shadow.xdist > glm::length(light - ray.xpt)) {
			colorSum += lighting;
		} else {

			// work out the color of light from passing through all transparent objects
			Ray sh = shadow;
			float reduction = 1.0f;
			glm::vec3 shColor = glm::vec3(1);
			int limit = MAX_STEPS;

			while (sh.xindex != -1 && limit-- != 0) {
				// exclude duplicates from exiting objects
				if (glm::dot(sceneObjects[sh.xindex]->normal(sh.xpt), sh.dir) < 0) {
					reduction *= (1.0f - sceneObjects[sh.xindex]->opacity);
					shColor *= sceneObjects[sh.xindex]->getColor(sh.xpt);
				}
				
				// create a new ray
				sh = Ray(sh.xpt, sh.dir);
				sh.closestPt(sceneObjects);
			}

			// mixin shadow color so it reduces with combined object opacity
			shColor += (glm::vec3(1) - shColor) * reduction;
			colorSum += reduction * lighting * shColor;
		}

		// reflections
		if (sceneObjects[ray.xindex]->reflectivity > 0.0 && step < MAX_STEPS) {
			glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
			Ray reflectedRay(ray.xpt, reflectedDir);
			glm::vec3 reflectedCol = trace(reflectedRay, step + 1);  // recurse
			colorSum = sceneObjects[ray.xindex]->reflectivity * reflectedCol + (1.0f - sceneObjects[ray.xindex]->reflectivity) * colorSum;
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
			float divX = cellX / ANTIALIAS_LEVEL;
			float divY = cellX / ANTIALIAS_LEVEL;

			float cornerx = xp + 0.5*divX;
			float cornery = yp + 0.5*divY;

			for (int l = 0; l < ANTIALIAS_LEVEL; l++) {
				for (int m = 0; m < ANTIALIAS_LEVEL; m++) {
					glm::vec3 dir(cornerx + l * divX, cornery + m * divY, -EDIST);	//direction of the primary ray
					Ray ray = Ray(eye, dir);		//Create a ray originating from the camera in the direction 'dir'
					ray.normalize();				//Normalize the direction of the ray to a unit vector
					col += trace(ray, 0);
				}
			}
			col /= ANTIALIAS_LEVEL*ANTIALIAS_LEVEL;  // average of extra rays

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

		// Sphere *sphere1 = new SphereTex(glm::vec3(-5.0, 0.0, -90.0), 15.0, moonFilename, glm::vec3(0, 0, 1));
	Sphere *sphere1 = new Sphere(glm::vec3(-7.0, 0.0, -100.0), 15.0, glm::vec3(0, 0, 1));	
	sphere1->reflectivity = .6f;

	// Cylinder *cylinder = new Cylinder(glm::vec3(7.0, -11.0, -80.0), 4.0, glm::vec3(1, 0, 0));
	// cylinder->reflectivity = 0.1;
	// Sphere *sphere2 = new Sphere(glm::vec3(3.0, 5.0, -80.0), 4.0, glm::vec3(1, 0, 0));
	// Sphere *sphere3 = new Sphere(glm::vec3(-10.0, -10.0, -82.0), 4.0, glm::vec3(0, 1, 0));
	// Sphere *moon = new SphereTex(glm::vec3(15.0, 15.0, -100.0), 6.0, moonFilename, glm::vec3(0, 0, 1));

	// Plane *plane = new Plane (glm::vec3(-200., -15, 1000),
    //                           glm::vec3(200., -15, 1000),
    //                           glm::vec3(200., -15, -1000),
    //                           glm::vec3(-200., -15, -1000),
    //                           glm::vec3(0.5, 0.5, 0),
	// 						  false);  // infinate plane
	// Cube *cube = new Cube(3, glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0));
	// cube->transform = glm::translate(cube->transform, glm::vec3(-14, 12, -80));
	// cube->transform = glm::rotate(cube->transform, PI/6, glm::vec3(0.5, 1, 0));

	// Sphere *glass = new Sphere(glm::vec3(-6.0, 10.0, -65.0), 5.0, glm::vec3(0, 1, 0));
	sphere1->refractiveIndex = 1/1.01f;
	sphere1->opacity = 0.2f;
	sphere1->reflectivity = 0.02f;

	// Torus *torus = new Torus(glm::vec3(0.0, 0.0, 0.0), 3.0, glm::vec3(0, 0, 1));

	// torus->transform = glm::translate(torus->transform, glm::vec3(20, 0, -130.0));
	// torus->transform = glm::rotate(torus->transform, -PI/4, glm::vec3(0, 1, 0));

	// sceneObjects.push_back(sphere1);
	// sceneObjects.push_back(sphere2);
	// sceneObjects.push_back(sphere3);
	// sceneObjects.push_back(moon);
	// sceneObjects.push_back(glass);

	// sceneObjects.push_back(cube); 
	// sceneObjects.push_back(cylinder);
	// sceneObjects.push_back(plane);
	// sceneObjects.push_back(torus);

		// Cylinder *cylinder = new Cylinder(glm::vec3(5.0, -10.0, -70.0), 4.0, glm::vec3(1, 0, 0));
	Sphere *sphere2 = new Sphere(glm::vec3(-5.0, 0.0, -90.0), 2.0, glm::vec3(1, 0, 0));
	// Sphere *sphere3 = new Sphere(glm::vec3(-6.0, -10.0, -78.0), 4.0, glm::vec3(0, 1, 0));
	// Torus *torus = new Torus(glm::vec3(0.0, 0.0, 0.0), 3.0, glm::vec3(0, 0, 1));
	Plane *plane = new Plane (glm::vec3(-200., -15, 1000),
                              glm::vec3(200., -15, 1000),
                              glm::vec3(200., -15, -1000),
                              glm::vec3(-200., -15, -1000),
                              glm::vec3(0.5, 0.5, 0),
							  false);  // infinate plane
	Cube *cube = new Cube(5, glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0));
	cube->transform = glm::translate(cube->transform, glm::vec3(0, -5, -80));
	cube->transform = glm::rotate(cube->transform, PI/8, glm::vec3(1, 1, 0));
	

	// torus->transform = glm::translate(torus->transform, glm::vec3(0, 5, -80.0));
	// torus->transform = glm::rotate(torus->transform, PI/4, glm::vec3(0, 1, 0));
	// torus->opacity = 0.2f;
	// torus->reflectivity = .1f;
	// torus->refractiveIndex = 1/1.01f;
	sceneObjects.push_back(sphere1);
	// sceneObjects.push_back(sphere2);
	// sceneObjects.push_back(cube);
	// sceneObjects.push_back(cylinder);
	// sceneObjects.push_back(sphere3);
	// sceneObjects.push_back(torus);
	sceneObjects.push_back(plane);


	// // Sphere *sphere1 = new SphereTex(glm::vec3(-5.0, 0.0, -90.0), 15.0, moonFilename, glm::vec3(0, 0, 1));
	// Sphere *sphere1 = new Sphere(glm::vec3(-10.0, 0.0, -120.0), 20.0, glm::vec3(0, 0, 1));	
	// // sphere1->opacity = 0.2f;
	// sphere1->reflectivity = .6f;
	// // sphere1->refractiveIndex = 1/1.01f;

	// Cylinder *cylinder = new Cylinder(glm::vec3(10.0, -16.0, -100.0), 4.0, glm::vec3(1, 0, 0));
	// Sphere *sphere2 = new Sphere(glm::vec3(-5.0, 0.0, -90.0), 2.0, glm::vec3(1, 0, 0));
	// // Sphere *sphere3 = new Sphere(glm::vec3(-6.0, -10.0, -78.0), 4.0, glm::vec3(0, 1, 0));
	// // Torus *torus = new Torus(glm::vec3(0.0, 0.0, 0.0), 3.0, glm::vec3(0, 0, 1));
	// Plane *plane = new Plane (glm::vec3(-200., -20, 1000),
    //                           glm::vec3(200., -20, 1000),
    //                           glm::vec3(200., -20, -1000),
    //                           glm::vec3(-200., -20, -1000),
    //                           glm::vec3(0.5, 0.5, 0),
	// 						  false);  // infinate plane
	// // Cube *cube = new Cube(5, glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0));
	// // cube->transform = glm::translate(cube->transform, glm::vec3(0, -5, -80));
	// // cube->transform = glm::rotate(cube->transform, PI/8, glm::vec3(1, 1, 0));
	

	// // torus->transform = glm::translate(torus->transform, glm::vec3(0, 0, -80.0));
	// // torus->transform = glm::rotate(torus->transform, 0.05f, glm::vec3(1, 0, 0));
	// // torus->opacity = 0.2f;
	// // torus->reflectivity = .1f;
	// // torus->refractiveIndex = 1/1.01f;
	// sceneObjects.push_back(sphere1);
	// sceneObjects.push_back(sphere2);
	// // sceneObjects.push_back(cube);
	// sceneObjects.push_back(cylinder);
	// // sceneObjects.push_back(sphere3);
	// // sceneObjects.push_back(torus);
	// sceneObjects.push_back(plane);

	// apply inverted operation
	for (auto o : sceneObjects) {
		o->transform = glm::inverse(o->transform);
	}
}


int main(int argc, char *argv[]) {
	cout << "Lab 8" << endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(NUMDIV, NUMDIV);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
