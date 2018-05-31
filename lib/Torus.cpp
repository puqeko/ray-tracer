/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The torus class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Torus.h"
#include "solve-quartic.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <complex>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>
using namespace std;


/**
* Torus's intersection method.  The input is a ray (pos, dir).
*/
float Torus::intersect(glm::vec3 posn, glm::vec3 dir)
{
	glm::vec4 tpt = this->transform * glm::vec4(posn.x, posn.y, posn.z, 1);
	glm::vec4 tdir = this->transform * glm::vec4(dir.x, dir.y, dir.z, 0);
	glm::vec3 d = glm::vec3(tdir.x, tdir.y, tdir.z);  //glm::vec3(0, 0, -1);

    glm::vec3 p = glm::vec3(tpt.x, tpt.y, tpt.z);
    double r = this->radius;

    // equation for a torus intersection with major radius = 3 * minor radius
    double a = d.x*d.x*d.x*d.x + d.y*d.y*d.y*d.y + d.z*d.z*d.z*d.z;
    a += 2*d.x*d.x*d.y*d.y + 2*d.x*d.x*d.z*d.z + 2*d.y*d.y*d.z*d.z;
    double b = 4*d.x*d.x*d.x*p.x + 4*d.x*d.x*d.y*p.y + 4*d.x*d.x*d.z*p.z + 4*d.x*d.y*d.y*p.x + 4*d.x*d.z*d.z*p.x;
    b += 4*d.y*d.y*d.y*p.y + 4*d.y*d.y*d.z*p.z + 4*d.y*d.z*d.z*p.y + 4*d.z*d.z*d.z*p.z;
    double c = -20*d.x*d.x*r*r + 6*d.x*d.x*p.x*p.x + 2*d.x*d.x*p.y*p.y + 2*d.x*d.x*p.z*p.z;
    c += 8*d.x*d.y*p.x*p.y + 8*d.x*d.z*p.x*p.z - 20*d.y*d.y*r*r + 2*d.y*d.y*p.x*p.x + 6*d.y*d.y*p.y*p.y;
    c += 2*d.y*d.y*p.z*p.z + 8*d.y*d.z*p.y*p.z + 16*d.z*d.z*r*r + 2*d.z*d.z*p.x*p.x;
    c += 2*d.z*d.z*p.y*p.y + 6*d.z*d.z*p.z*p.z;
    double d_ = -40*d.x*r*r*p.x + 4*d.x*p.x*p.x*p.x + 4*d.x*p.x*p.y*p.y + 4*d.x*p.x*p.z*p.z;
    d_ += -40*d.y*r*r*p.y + 4*d.y*p.x*p.x*p.y + 4*d.y*p.y*p.z*p.z + 32*d.z*r*r*p.z;
    d_ += 4*d.z*p.x*p.x*p.z + 4*d.z*p.y*p.y*p.z + 4*d.z*p.z*p.z*p.z;
    double e = 64*r*r*r*r - 20*r*r*p.x*p.x - 20*r*r*p.y*p.y + 16*r*r*p.z*p.z;
    e += p.x*p.x*p.x*p.x + 2*p.x*p.x*p.y*p.y + 2*p.x*p.x*p.z*p.z + p.y*p.y*p.y*p.y + 2*p.y*p.y*p.z*p.z + p.z*p.z*p.z*p.z;

    std::complex<double> coeff[5] = {e + 0i, d_ + 0i, c + 0i, b + 0i, a + 0i};
    std::complex<double> sols[4];
    solve_quartic(coeff, sols);

    std::vector<double> roots;
    for (int i = 0; i < 4; i++) {
        if (abs(sols[i].imag()) < 0.001 && sols[i].real() > 0.001) {
            roots.push_back(sols[i].real());
        }
    }

    // no real +ve roots?
    if (roots.empty()) return -1.0;

    // else, one of them is the answer
    std::sort(roots.begin(), roots.end());
    // cout << roots[0] << endl;
    return roots[0];
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the torus.
*/
glm::vec3 Torus::normal(glm::vec3 pt)
{
    glm::vec4 tpt = this->transform * glm::vec4(pt.x, pt.y, pt.z, 1);
	glm::vec3 p = glm::vec3(tpt.x, tpt.y, tpt.z);

    glm::vec3 pFlat = p; pFlat.z = 0;
    glm::vec3 n = pFlat;

    // point towards point by the radius to find center of torus loop
    n = glm::normalize(n) * this->radius * 3.0f;  // R = r * 3

    // return direction that point is from the major radial line
    glm::vec3 local = glm::normalize(p - n);
    glm::vec4 l = glm::vec4(local.x, local.y, local.z, 0);
    glm::vec4 res = glm::inverse(this->transform) * l;
    return glm::vec3(res.x, res.y, res.z);
}
