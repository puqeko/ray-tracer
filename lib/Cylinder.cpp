/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
* This cylinder is not orientable.
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>

bool Cylinder::inBounds(glm::vec3 pt) {
    glm::vec3 centerDiff = pt - center;
    return fabs(centerDiff.y) < this->radius;
}

float Cylinder::calcCapT(glm::vec3 p0, glm::vec3 dir) {
    float norm = ((dir.y < 0) ? 1.0 : -1.0);
    float targetHeight = this->center.y + this->radius * norm;
    return (targetHeight - p0.y) / dir.y;
}

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

    if (discrim < 0.001) return -1.0; 

    // t1 is smallest
    float t1 = (-b - sqrt(discrim)) / a;
    float t2 = (-b + sqrt(discrim)) / a;
    
    // cylinder is behind the viewer, so ignore
    if (t1 < 0.001 && t2 < 0.001) return -1.0;

    glm::vec3 pt1 = posn + t1 * dir;
    glm::vec3 pt2 = posn + t2 * dir;

    // if closest point is valid then return that point
    if (inBounds(pt1)) return t1;

    // inside cylinder
    if (t1 < 0.001 && inBounds(pt2)) return t2;

    // test if we are looking through the cylinder bounds over top
    // or underneath. If not, we are looking down or up through the
    // cylinder so find the correct point on the cylinder cap.
    float diff1 = pt1.y - this->center.y;
    float diff2 = pt2.y - this->center.y;

    if (fabs(diff1) < 0.001 && fabs(diff2) < 0.001) return -1.0;

    float norm1 = (diff1 > 0) ? 1.0 : -1.0;
    float norm2 = (diff2 < 0) ? -1.0 : 1.0;
    if (norm1 == norm2 && !inBounds(pt2)) return -1.0;
    return calcCapT(posn, dir);
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 n = p - center;

    // check if the point is on one of the caps
    float dist = n.x*n.x + n.z*n.z - this->radius*this->radius;
    if (dist < -0.001 && n.y > this->radius - 0.001) {
        return glm::vec3(0, (n.y < 0) ? -1 : 1, 0);  // return up or down
    }

    // else, return a normal for the side of the cylinder
    n.y = 0;
    n = glm::normalize(n);
    return n;
}
