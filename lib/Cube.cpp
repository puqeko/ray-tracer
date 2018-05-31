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
* Cube's intersection method.  The input is a ray (pos, dir). 
*/
float Cube::intersect(glm::vec3 posn, glm::vec3 dir)
{
	glm::vec4 tpt = this->transform * glm::vec4(posn.x, posn.y, posn.z, 1);
	posn = glm::vec3(tpt.x, tpt.y, tpt.z);

	glm::vec4 tdir = this->transform * glm::vec4(dir.x, dir.y, dir.z, 0);
	dir = glm::vec3(tdir.x, tdir.y, tdir.z);

	float tmin = -1;
	int idx = -1;
	for (int i = 0; i < faces.size(); i++) {
		float t = this->faces[i]->intersect(posn, dir);
	    if (t > 0) {
			if (tmin < 0) {
				tmin = t;
				idx = i;
			} else if (t < tmin) {
				tmin = t;
				idx = i;
			}
		}
	}
	this->activeFace = idx;
	return tmin;
}

glm::vec3 Cube::normal(glm::vec3 pt)
{
	glm::vec4 tpt = this->transform * glm::vec4(pt.x, pt.y, pt.z, 1);
	pt = glm::vec3(tpt.x, tpt.y, tpt.z);

	if (this->activeFace == -1) {
		std::cout << "'normal' must call intersect before normal" << std::endl;
		exit(1);
	}

	return this->faces[this->activeFace]->normal(pt);
}