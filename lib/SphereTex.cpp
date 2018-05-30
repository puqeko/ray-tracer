/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere textured class
*  This is a subclass of Sphere displaying a texture on it's surface
-------------------------------------------------------------*/

#include "SphereTex.h"
#include <math.h>

#include <iostream>
using namespace std;

#define PI 3.14159265358979f

// triangle with short sides a and b, and long side c
float arcsin(float a, float b, float c)
{
    if (a > 0) return asin(b/c);

    // handle case where in left two quadrants
    return PI - asin(b/c);
}

glm::vec3 SphereTex::getColor(glm::vec3 pt)
{
    // transform to texture coordinates and return color
    glm::vec3 relPt = pt - this->center;
    float R = glm::length(relPt);
    float Rxz = sqrt(relPt.x*relPt.x + relPt.z*relPt.z);
    float phi = arcsin(relPt.x, relPt.z, Rxz);
    float theta = arcsin(Rxz, relPt.y, R);
    float s = 0.5*phi/PI + 0.25f;
    float t = theta/PI + 0.5f;
    return this->tex->getColorAt(s, t);
}