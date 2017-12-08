//
// Created by baian on 11/27/17.
//

#include <Matrix3f.h>
#include <cmath>
#include <random>
#include "Samplers.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-1.0, 1.0);

std::vector<Ray> NaiveSampler::sample(Ray inR, Hit h, int num) {
    auto normal = h.getNormal();
    auto origin = inR.pointAtParameter(h.getT());
    auto ret = std::vector<Ray>();
    ret.push_back(Ray(origin, Matrix3f::rotateX(0.1f)*normal));
    ret.push_back(Ray(origin, Matrix3f::rotateX(-0.1f)*normal));
    ret.push_back(Ray(origin, Matrix3f::rotateY(0.1f)*normal));
    ret.push_back(Ray(origin, Matrix3f::rotateY(-0.1f)*normal));
    return ret;
}

std::vector<Ray> MonteCarloSampler::sample(Ray inR, Hit h, int num) {
    auto normal = h.getNormal().normalized();
    auto origin = inR.pointAtParameter(h.getT());
    auto ret = std::vector<Ray>();
    if (isnan(normal.x()) or isnan(normal.x()) or isnan(normal.x())) {
        return ret;
    }
    for(int i = 0; i < num; i++) {
        Vector3f dir;
        while(true) {
            float a = dis(gen);
            float b = dis(gen);
            float c = dis(gen);
            //std::cout << a << " " << b << " " << c << std::endl;
            //std::cout << normal.x() << " " << normal.y() << " " << normal.z() << std::endl;
            dir = Vector3f(a,b,c).normalized();
            //std::cout << Vector3f::dot(dir, normal) << std::endl;
            if (Vector3f::dot(dir, normal) > 0) {
                break;
            }
        }
        ret.push_back(Ray(origin, dir));
    }
    return ret;
}

float d(Vector3f vm, Vector3f n, float m=0.8) {

    auto h = vm;
    auto ndoth = Vector3f::dot(n,h);
    if(ndoth <= 0) {
        return 0;
    }
    auto D = 1.0f / (m * m) / pow(ndoth, 4) * exp((ndoth * ndoth - 1)/(m * m * ndoth * ndoth));
    return D;

}

std::vector<Ray> ImportanceSampler::sample(Ray inR, Hit h, int num) {
    auto normal = h.getNormal().normalized();
    auto origin = inR.pointAtParameter(h.getT());
    auto ret = std::vector<Ray>();
    if (isnan(normal.x()) or isnan(normal.x()) or isnan(normal.x())) {
        return ret;
    }
    for(int i = 0; i < num; i++) {
        Vector3f o;
        bool flag = false;
        for (int j = 0; j < 20; j++) {
            auto m = Vector3f(dis(gen),dis(gen),dis(gen)).normalized();
            if (Vector3f::dot(m, normal) > 0) {
                o = 2 * Vector3f::dot(-inR.getDirection().normalized(), m) * m + inR.getDirection().normalized();
                float p = d(m, normal) * Vector3f::dot(m, normal) / 4.0f / Vector3f::dot(o,m);
                if (((double) rand() / (RAND_MAX)) <= p) {
                    flag = true;
                    break;
                }
            }
        }
        if (flag) {
            ret.push_back(Ray(origin, o));
        } else {
            Vector3f dir;
            while(true) {
                dir = Vector3f(dis(gen),dis(gen),dis(gen)).normalized();
                if (Vector3f::dot(dir, normal) > 0) {
                    break;
                }
            }
            ret.push_back(Ray(origin, dir));
        }
    }
    return ret;
}
