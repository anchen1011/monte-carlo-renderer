//
// Created by baian on 11/27/17.
//

#ifndef A4_SAMPLERS_H
#define A4_SAMPLERS_H


#include <vector>
#include "Ray.h"


class NaiveSampler {
public:
    static std::vector<Ray> sample(Ray inR, Hit h, int num=4);
};

class MonteCarloSampler {
public:
    static std::vector<Ray> sample(Ray inR, Hit h, int num=4);
};

class ImportanceSampler {
public:
    static std::vector<Ray> sample(Ray inR, Hit h, int num=4);
};

#endif //A4_SAMPLERS_H
