/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_CYLINDER
#define H_CYLINDER
#include <glm/glm.hpp>
#include "SceneObject.h"

/**
 * Defines a simple Sphere located at 'center' 
 * with the specified radius
 */
class Cylinder : public SceneObject
{

private:
    glm::vec3 center;
    float radius;

	bool inBounds(glm::vec3 pt);

public:	
	Cylinder()
		: center(glm::vec3(0)), radius(1)  //Default constructor creates a unit sphere
	{
		color = glm::vec3(1);
	};
	
    Cylinder(glm::vec3 c, float r, glm::vec3 col)
		: center(c), radius(r)
	{
		color = col;
	};

	float intersect(glm::vec3 posn, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);

};

#endif //!H_CYLINDER
