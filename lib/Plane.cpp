/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Plane.h"
#include <math.h>


/**
* Checks if a point pt is inside the current polygon
* Implement a point inclusion test using 
* member variables a, b, c, d.
*/
bool Plane::isInside(glm::vec3 pt)
{
	glm::vec3 ua = b - a, ub = c - b, uc = d - c, ud = a - d;
	glm::vec3 va = pt - a, vb = pt - b, vc = pt - c, vd = pt - d;
	glm::vec3 n = normal(pt);

	if (glm::dot(glm::cross(ua, va), n) < 0) return false;
	if (glm::dot(glm::cross(ub, vb), n) < 0) return false;
	if (glm::dot(glm::cross(uc, vc), n) < 0) return false;
	if (glm::dot(glm::cross(ud, vd), n) < 0) return false;
	return true;
}

/**
* Plane's intersection method.  The input is a ray (pos, dir). 
*/
float Plane::intersect(glm::vec3 posn, glm::vec3 dir)
{
	glm::vec3 n = normal(posn);
	glm::vec3 vdif = a - posn;
	float vdotn = glm::dot(dir, n);
	if(fabs(vdotn) < 1.e-4) return -1;
    float t = glm::dot(vdif, n)/vdotn;
	if(fabs(t) < 0.0001) return -1;
	glm::vec3 q = posn + dir*t;
	if(isInside(q) || this->isInfinate) return t;
    else return -1;
}

/**
* Returns the unit normal vector at a given point.
* Compute the plane's normal vector using 
* member variables a, b, c, d.
* The parameter pt is a dummy variable and is not used.
*/
glm::vec3 Plane::normal(glm::vec3 pt)
{
	glm::vec3 n = glm::vec3(0);

	n = glm::cross(glm::normalize(b - a), glm::normalize(d - a));

    return n;
}

glm::vec3 Plane::getColor(glm::vec3 pt)
{
	bool isxEven = (int)(pt.x/10) % 2 == 0;
	if (pt.x < 0) isxEven = !isxEven;

	bool isyEven = (int)(pt.z/10) % 2 == 0;
	if (pt.y < 0) isyEven = !isyEven;

	if (isxEven) {
		return (isyEven) ? glm::vec3(0, 0, 0) : glm::vec3(1, 1, 1);
	} else {
		return (isyEven) ? glm::vec3(1, 1, 1) : glm::vec3(0, 0, 0);
	}
}