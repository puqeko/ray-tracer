/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The torus class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Torus.h"
#include "solve-quartic.h"
#include <math.h>
#include <vector>
#include <iostream>
using namespace std;


/**
* Torus's intersection method.  The input is a ray (pos, dir).
*/
float Torus::intersect(glm::vec3 posn, glm::vec3 dir)
{
	glm::vec4 tpt = this->transform * glm::vec4(posn.x, posn.y, posn.z, 1);
	posn = glm::vec3(tpt.x, tpt.y, tpt.z);

	glm::vec4 tdir = this->transform * glm::vec4(dir.x, dir.y, dir.z, 0);
	glm::vec3 d = glm::vec3(tdir.x, tdir.y, tdir.z);

    glm::vec3 p = posn;
    float r = this->radius;

    // equation for a torus intersection with major radius = 3 * minor radius

    // flat
    // float a = pow(d.x, 4) + pow(d.y, 4) + pow(d.z, 4);
    // a += 2*pow(d.x, 2)*pow(d.y, 2) + 2*pow(d.x, 2)*pow(d.z, 2) + 2*pow(d.y, 2)*pow(d.z, 2);
    // float b = 4*pow(d.x, 3)*p.x + 4*pow(d.x, 2)*d.y*p.y + 4*pow(d.x, 2)*d.z*p.z + 4*d.x*pow(d.y, 2)*p.x + 4*d.x*pow(d.z, 2)*p.x;
    // b += 4*pow(d.y, 3)*p.y + 4*pow(d.y, 2)*d.z*p.z + 4*d.y*pow(d.z, 2)*p.y + 4*pow(d.z, 3)*p.z;
    // float c = -20*pow(d.x, 2)*pow(r, 2) + 6*pow(d.x, 2)*pow(p.x, 2) + 2*pow(d.x, 2)*pow(p.y, 2) + 2*pow(d.x, 2)*pow(p.z, 2);
    // c += 8*d.x*d.y*p.x*p.y + 8*d.x*d.z*p.x*p.z + 16*pow(d.y, 2)*pow(r, 2) + 2*pow(d.y, 2)*pow(p.x, 2) + 6*pow(d.y, 2)*pow(p.y, 2);
    // c += 2*pow(d.y, 2)*pow(p.z, 2) + 8*d.y*d.z*p.y*p.z - 20*pow(d.z, 2)*pow(r, 2) + 2*pow(d.z, 2)*pow(p.x, 2);
    // c += 2*pow(d.z, 2)*pow(p.y, 2) + 6*pow(d.z, 2)*pow(p.z, 2);
    // float d_ = -40*d.x*pow(r, 2)*p.x + 4*d.x*pow(p.x, 3) + 4*d.x*p.x*pow(p.y, 2) + 4*d.x*p.x*pow(p.z, 2);
    // d_ += 32*d.y*pow(r, 2)*p.y + 4*d.y*pow(p.x, 2)*p.y + 4*d.y*p.y*pow(p.z, 2) - 40*d.z*pow(r, 2)*p.z;
    // d_ += 4*d.z*pow(p.x, 2)*p.z + 4*d.z*pow(p.y, 2)*p.z + 4*d.z*pow(p.z, 3);
    // float e = 64*pow(r, 4) - 20*pow(r, 2)*pow(p.x, 2) + 16*pow(r, 2)*pow(p.y, 2) - 20*pow(r, 2)*pow(p.z, 2);
    // e += pow(p.x, 4) + 2*pow(p.x, 2)*pow(p.y, 2) + 2*pow(p.x, 2)*pow(p.z, 2) + pow(p.y, 4) + 2*pow(p.y, 2)*pow(p.z, 2) + pow(p.z, 4);

    // upright
    float a = pow(d.x, 4) + pow(d.y, 4) + pow(d.z, 4);
    a += 2*pow(d.x, 2)*pow(d.y, 2) + 2*pow(d.x, 2)*pow(d.z, 2) + 2*pow(d.y, 2)*pow(d.z, 2);
    float b = 4*pow(d.x, 3)*p.x + 4*pow(d.x, 2)*d.y*p.y + 4*pow(d.x, 2)*d.z*p.z + 4*d.x*pow(d.y, 2)*p.x + 4*d.x*pow(d.z, 2)*p.x;
    b += 4*pow(d.y, 3)*p.y + 4*pow(d.y, 2)*d.z*p.z + 4*d.y*pow(d.z, 2)*p.y + 4*pow(d.z, 3)*p.z;
    float c = -20*pow(d.x, 2)*pow(r, 2) + 6*pow(d.x, 2)*pow(p.x, 2) + 2*pow(d.x, 2)*pow(p.y, 2) + 2*pow(d.x, 2)*pow(p.z, 2);
    c += 8*d.x*d.y*p.x*p.y + 8*d.x*d.z*p.x*p.z - 20*pow(d.y, 2)*pow(r, 2) + 2*pow(d.y, 2)*pow(p.x, 2) + 6*pow(d.y, 2)*pow(p.y, 2);
    c += 2*pow(d.y, 2)*pow(p.z, 2) + 8*d.y*d.z*p.y*p.z + 16*pow(d.z, 2)*pow(r, 2) + 2*pow(d.z, 2)*pow(p.x, 2);
    c += 2*pow(d.z, 2)*pow(p.y, 2) + 6*pow(d.z, 2)*pow(p.z, 2);
    float d_ = -40*d.x*pow(r, 2)*p.x + 4*d.x*pow(p.x, 3) + 4*d.x*p.x*pow(p.y, 2) + 4*d.x*p.x*pow(p.z, 2);
    d_ += -40*d.y*pow(r, 2)*p.y + 4*d.y*pow(p.x, 2)*p.y + 4*d.y*p.y*pow(p.z, 2) + 32*d.z*pow(r, 2)*p.z;
    d_ += 4*d.z*pow(p.x, 2)*p.z + 4*d.z*pow(p.y, 2)*p.z + 4*d.z*pow(p.z, 3);
    float e = 64*pow(r, 4) - 20*pow(r, 2)*pow(p.x, 2) - 20*pow(r, 2)*pow(p.y, 2) + 16*pow(r, 2)*pow(p.z, 2);
    e += pow(p.x, 4) + 2*pow(p.x, 2)*pow(p.y, 2) + 2*pow(p.x, 2)*pow(p.z, 2) + pow(p.y, 4) + 2*pow(p.y, 2)*pow(p.z, 2) + pow(p.z, 4);

    std::complex<double> coeff[5] = {e + 0i, d_ + 0i, c + 0i, b + 0i, a + 0i};
    std::complex<double> sols[4];
    solve_quartic(coeff, sols);

    std::vector<double> roots;
    for (int i = 0; i < 4; i++) {
        if (sols[i].imag() == 0.0 && sols[i].real() >= 0) {  // sols[i].real() > 0.001
            roots.push_back(sols[i].real());
            // cout << sols[i].real() << ' ';
        }
    }

    // std::complex<double>* sols = solve_quartic(b/a, c/a, d_/a, e/a);
    // std::vector<double> roots;

    // // select real and +ve only
    // for (int i = 0; i < 4; i++) {
    //     if (abs(sols[i].imag()) < 0.001 && sols[i].real() > 0) {  // sols[i].real() > 0.001
    //         roots.push_back(sols[i].real());
    //     }
    // }
    // delete[] sols;  // clear memory

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
    return glm::inverse(this->transform) * l;
}
