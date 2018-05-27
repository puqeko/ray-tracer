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
	if (this->activeFace == -1) {
		std::cout << "'normal' must call intersect before normal" << std::endl;
		exit(1);
	}

	return this->faces[this->activeFace]->normal(pt);
}