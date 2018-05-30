/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Sphere.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray (pos, dir). 
*/
float Sphere::intersect(glm::vec3 posn, glm::vec3 dir)
{
    glm::vec3 vdif = posn - center;
    // because a = 1
    float b = glm::dot(dir, vdif);
    float len = glm::length(vdif);
    float c = len*len - radius*radius;
    float delta = b*b - c;
   
    if(delta < 0.001) return -1.0;

    // t1 < t2
    float t1 = (-b - sqrt(delta));
    float t2 = (-b + sqrt(delta));

    if (t1 < 0.001 && t2 < 0.001) return -1.0;

    if (t1 < 0.001) {
        if (t2 < 0.001) return -1.0;
        return t2;
    }
    return t1;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Sphere::normal(glm::vec3 p)
{
    glm::vec3 n = p - center;
    n = glm::normalize(n);
    return n;
}
