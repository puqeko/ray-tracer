/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_PLANE
#define H_PLANE

#include <glm/glm.hpp>
#include "SceneObject.h"

class Plane : public SceneObject
{
private:
    glm::vec3 a, b, c, d;  //The four vertices
	bool isInfinate = false;

public:	
	Plane(void);
	
    Plane(glm::vec3 pa, glm::vec3 pb, glm::vec3 pc, glm::vec3 pd, glm::vec3 col, bool infinate)
		: a(pa), b(pb), c(pc), d(pd), isInfinate(infinate)
	{
		color = col;
	};

	bool isInside(glm::vec3 pt);
	float intersect(glm::vec3 posn, glm::vec3 dir);
	glm::vec3 normal(glm::vec3 pt);
	glm::vec3 getColor(glm::vec3 pt);
};

#endif //!H_PLANE
