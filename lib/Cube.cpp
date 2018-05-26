/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cube.h"
#include <math.h>
#include <iostream>

/**
* Returns the unit normal vector at a given point.
* Compute the plane's normal vector using 
* member variables a, b, c, d.
* The parameter pt is a dummy variable and is not used.
*/
glm::vec3 normalp(glm::vec3 pt, std::vector<glm::vec3> plane)
{
	// verts
	glm::vec3 a = plane[0], b = plane[1], c = plane[2], d = plane[3];

	glm::vec3 n = glm::vec3(0);
	n = glm::cross(glm::normalize(b - a), glm::normalize(d - a));
    return n;
}

/**
* Checks if a point pt is inside the current polygon
* Implement a point inclusion test using 
* member variables a, b, c, d.
*/
bool Cube::isInside(glm::vec3 pt, std::vector<glm::vec3> plane)
{
	glm::vec3 a = plane[0], b = plane[1], c = plane[2], d = plane[3];
	glm::vec3 ua = b - a, ub = c - b, uc = d - c, ud = a - d;
	glm::vec3 va = pt - a, vb = pt - b, vc = pt - c, vd = pt - d;
	glm::vec3 n = normalp(pt, plane);

	if (glm::dot(glm::cross(ua, va), n) < 0) return false;
	if (glm::dot(glm::cross(ub, vb), n) < 0) return false;
	if (glm::dot(glm::cross(uc, vc), n) < 0) return false;
	if (glm::dot(glm::cross(ud, vd), n) < 0) return false;
	return true;
}

/**
* Plane's intersection method.  The input is a ray (pos, dir). 
*/
float Cube::intersect(glm::vec3 posn, glm::vec3 dir)
{
	float tmin = -1;
	int idx = -1;
	for (int i = 0; i < faces.size(); i++) {
		std::vector<glm::vec3> plane = faces[i];
		glm::vec3 n = normalp(posn, plane);
		glm::vec3 vdif = plane[0] - posn;
		float vdotn = glm::dot(dir, n);
		if (fabs(vdotn) < 1.e-4) continue;
		float t = glm::dot(vdif, n)/vdotn;
		if (fabs(t) < 0.0001) continue;
		glm::vec3 q = posn + dir*t;
		if (isInside(q, plane)) {
			tmin = t;
			idx = i;
		}
	}

	if (idx != -1) {
		this->activeFace = idx;
		return tmin;
	}
	return -1;
}

glm::vec3 Cube::normal(glm::vec3 pt)
{
	if (this->activeFace == -1) {
		std::cout << "Must call intersect before normal" << std::endl;
		exit(1);
	}

	return normalp(pt, this->faces[this->activeFace]);
}