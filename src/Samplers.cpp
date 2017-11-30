//
// Created by baian on 11/27/17.
//

#include <Matrix3f.h>
#include "Samplers.h"

std::vector<Ray> NaiveSampler::sample(Ray inR, Hit h, int num) {
    auto normal = h.getNormal();
    auto origin = inR.pointAtParameter(h.getT());
    auto ret = std::vector<Ray>();
    ret.push_back(Ray(origin, Matrix3f::rotateX(0.1f)*normal));
    ret.push_back(Ray(origin, Matrix3f::rotateX(-0.1f)*normal));
    ret.push_back(Ray(origin, Matrix3f::rotateY(0.1f)*normal));
    ret.push_back(Ray(origin, Matrix3f::rotateY(0.1f)*normal));
    return ret;
}

std::vector<Ray> MonteCarloSampler::sample(Ray inR, Hit h, int num) {
    auto normal = h.getNormal();
    auto origin = inR.pointAtParameter(h.getT());
    auto ret = std::vector<Ray>();
    for(int i = 0; i < num; i++) {
        Vector3f dir;
        while(true) {
            dir = Vector3f(rand(),rand(),rand()).normalized();
            if (Vector3f::dot(dir, normal) > 0) {
                break;
            }
        }
        ret.push_back(Ray(origin, dir));
    }
    return ret;
}
