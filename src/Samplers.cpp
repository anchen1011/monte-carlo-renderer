//
// Created by baian on 11/27/17.
//

#include <Matrix3f.h>
#include "Samplers.h"

std::vector<Ray> NaiveSampler::sample(Ray inR, Hit h) {
    auto normal = h.getNormal();
    auto origin = inR.pointAtParameter(h.getT());
    auto ret = std::vector<Ray>();
    ret.push_back(Ray(origin, Matrix3f::rotateX(0.1f)*normal));
    ret.push_back(Ray(origin, Matrix3f::rotateX(-0.1f)*normal));
    ret.push_back(Ray(origin, Matrix3f::rotateY(0.1f)*normal));
    ret.push_back(Ray(origin, Matrix3f::rotateY(0.1f)*normal));
    return ret;
}
