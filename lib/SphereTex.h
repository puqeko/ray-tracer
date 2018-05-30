/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef H_SPHERETEX
#define H_SPHERETEX
#include <glm/glm.hpp>
#include <string>
#include "Sphere.h"
#include "TextureBMP.h"

/**
 * Defines a simple Sphere located at 'center' 
 * with the specified radius
 */
class SphereTex : public Sphere
{

private:
	char* filename;
	TextureBMP* tex;

public:	
    SphereTex(glm::vec3 c, float r, char* file, glm::vec3 col)
	{
		this->center = c;
		this->radius = r;
		this->filename = file;
		this->color = col;
		this->tex = new TextureBMP(filename);
	};

	glm::vec3 getColor(glm::vec3 pt);

};

#endif //!H_SPHERETEX
