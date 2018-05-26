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
#include "SceneObject.h"

class Cube : public SceneObject
{
private:
    float s;
	std::vector<std::vector<glm::vec3> > faces;
	int activeFace = -1;

public:	
	Cube(void);
	
    Cube(float size, glm::vec3 col)
		: s(size)
	{
		color = col;
		std::vector<glm::vec3> front;
		front.push_back(glm::vec3(-s, -s, s));
		front.push_back(glm::vec3(s, -s, s));
		front.push_back(glm::vec3(s, s, s));
		front.push_back(glm::vec3(-s, s, s));
		faces.push_back(front);

		// std::vector<glm::vec3> back;
		// back.push_back(glm::vec3(-s, -s, -s));
		// back.push_back(glm::vec3(-s, s, -s));
		// back.push_back(glm::vec3(s, s, -s));
		// back.push_back(glm::vec3(s, -s, -s));
		// faces.push_back(back);

		// std::vector<glm::vec3> left;
		// left.push_back(glm::vec3(-s, -s, -s));
		// left.push_back(glm::vec3(-s, -s, s));
		// left.push_back(glm::vec3(-s, s, s));
		// left.push_back(glm::vec3(-s, s, -s));
		// faces.push_back(left);

		// std::vector<glm::vec3> right;
		// right.push_back(glm::vec3(s, -s, -s));
		// right.push_back(glm::vec3(s, s, -s));
		// right.push_back(glm::vec3(s, s, s));
		// right.push_back(glm::vec3(s, -s, s));
		// faces.push_back(right);

		// std::vector<glm::vec3> top;
		// top.push_back(glm::vec3(-s, s, -s));
		// top.push_back(glm::vec3(-s, s, s));
		// top.push_back(glm::vec3(s, s, s));
		// top.push_back(glm::vec3(s, s, -s));
		// faces.push_back(top);

		// std::vector<glm::vec3> bot;
		// bot.push_back(glm::vec3(-s, -s, -s));
		// bot.push_back(glm::vec3(s, -s, -s));
		// bot.push_back(glm::vec3(s, -s, s));
		// bot.push_back(glm::vec3(-s, -s, s));
		// faces.push_back(bot);
	};

	bool isInside(glm::vec3 pt, std::vector<glm::vec3> plane);
	
	float intersect(glm::vec3 posn, glm::vec3 dir);
	
	glm::vec3 normal(glm::vec3 pt);

};

#endif //!CUBE_OBJ
