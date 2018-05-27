/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Plane class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#ifndef CUBE_OBJ
#define CUBE_OBJ

#include <vector>
#include <glm/glm.hpp>
#include "Plane.h"
#include "SceneObject.h"

class Cube : public SceneObject
{
private:
    float s;
	glm::vec3 c;
	int activeFace = -1;
	std::vector<Plane*> faces;

public:	
	Cube(void);
	
    Cube(float size, glm::vec3 center, glm::vec3 col)
		: s(size), c(center)
	{
		this->color = col;

		
		// color = col;
		// std::vector<glm::vec3> front;
		// front.push_back(glm::vec3(-s, -s, s));
		// front.push_back(glm::vec3(s, -s, s));
		// front.push_back(glm::vec3(s, s, s));
		// front.push_back(glm::vec3(-s, s, s));
		// faces.push_back(front);

		// std::vector<glm::vec3> back;
		// back.push_back(glm::vec3(-s, -s, -s));
		// back.push_back(glm::vec3(-s, s, -s));
		// back.push_back(glm::vec3(s, s, -s));
		// back.push_back(glm::vec3(s, -s, -s));
		// faces.push_back(back);

		// top
		faces.push_back(new Plane(c + glm::vec3(-s, s, s),
								  c + glm::vec3(s, s, s),
						          c + glm::vec3(s, s, -s),
						          c + glm::vec3(-s, s, -s),
						          col, false));

		// bot
		faces.push_back(new Plane(c + glm::vec3(-s, -s, -s),
								  c + glm::vec3(s, -s, -s),
								  c + glm::vec3(s, -s, s),
								  c + glm::vec3(-s, -s, s),
								  col, false));

		// left
		faces.push_back(new Plane(c + glm::vec3(-s, -s, -s),
							      c + glm::vec3(-s, -s, s),
							      c + glm::vec3(-s, s, s),
							      c + glm::vec3(-s, s, -s),
							      col, false));

		// right
		faces.push_back(new Plane(c + glm::vec3(s, s, -s),
							      c + glm::vec3(s, s, s),
							      c + glm::vec3(s, -s, s),
							      c + glm::vec3(s, -s, -s),
							      col, false));

		// back
		faces.push_back(new Plane(c + glm::vec3(s, -s, -s),
							      c + glm::vec3(-s, -s, -s),
							      c + glm::vec3(-s, s, -s),
							      c + glm::vec3(s, s, -s),
							      col, false));

		// front
	    faces.push_back(new Plane(c + glm::vec3(s, s, s),
							      c + glm::vec3(-s, s, s),
							      c + glm::vec3(-s, -s, s),
							      c + glm::vec3(s, -s, s),
							      col, false));
	};

	float intersect(glm::vec3 posn, glm::vec3 dir);
	glm::vec3 normal(glm::vec3 pt);
};

#endif //!CUBE_OBJ

	// };
