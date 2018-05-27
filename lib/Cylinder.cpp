/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray (pos, dir). 
*/
float Cylinder::intersect(glm::vec3 posn, glm::vec3 dir)
{
    // NOTE: a factor of 2 has been removed from b to simplify
    // calcuatlions.

    glm::vec3 diff = posn - center;
    float a = dir.x*dir.x + dir.z*dir.z;
    float b = dir.x*diff.x + dir.z*diff.z;
    float c = diff.x*diff.x + diff.z*diff.z - radius*radius;
    float discrim = b*b - a*c;

    if(fabs(discrim) < 0.001) return -1.0; 
    if(discrim < 0.0) return -1.0;

    float t1 = (-b - sqrt(discrim)) / a;
    float t2 = (-b + sqrt(discrim)) / a;

    if(fabs(t1) < 0.001 ) t1 = -1.0;
    if(fabs(t2) < 0.001 ) t2 = -1.0;

    return (t1 < t2)? t1: t2;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 n = p - center;
    n.y = 0;
    n = glm::normalize(n);
    return n;
}
