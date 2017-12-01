#include "Material.h"

float clamp(Vector3f a, Vector3f b) {
    float tmp = Vector3f::dot(a,b);
    if (tmp > 0) {
        return tmp;
    }
    return 0;
}

Vector3f Material::shade(const Ray &ray,
    const Hit &hit,
    const Vector3f &dirToLight,
    const Vector3f &lightIntensity)
{
    auto e = -ray.getDirection().normalized();
    auto n = hit.getNormal().normalized();
    auto r = - e + (2 * Vector3f::dot(e, n.normalized()) * n.normalized());
    auto l = dirToLight.normalized();
    auto iDiffuse = clamp(l,n) * lightIntensity;
    auto iSpecular = pow(clamp(l,r), _shininess) * lightIntensity;
    for (unsigned i = 0; i < 3; i++) {
        iDiffuse[i] *= _diffuseColor[i];
        iSpecular[i] *= _specularColor[i];
    }
    return iDiffuse + iSpecular;
}

float Material::shadeBrdf(const Ray &ray,
                             const Ray &outRay,
                         const Hit &hit)
{
    //return 1.0f;

    float m = 0.2;
    auto wi = ray.getDirection().normalized();
    auto wo = outRay.getDirection().normalized();
    auto n = hit.getNormal().normalized();

    auto e = -ray.getDirection().normalized();
    auto r = - e + (2 * Vector3f::dot(e, n.normalized()) * n.normalized());

    auto c = pow(clamp(wo,r), _shininess);
    auto h = (wi + wo) / (wi + wo).abs();
    auto ndoth = Vector3f::dot(n,h);
    auto D = 1 / (0.2 * 0.2) * exp(((ndoth * ndoth)-1)/(m * m * ndoth * ndoth));
    auto F = (1-c) * pow((1-Vector3f::dot(wi, h)),5) + c;
    auto G = std::fminf(1, 2 * ndoth * Vector3f::dot(n, wo) / Vector3f::dot(wo, h));
    G = std::fminf(G, 2 * ndoth * Vector3f::dot(n, wo) / Vector3f::dot(wi, h));
    return fmaxf(0, D * F * G / 3.1415926 / Vector3f::dot(n ,wi) / Vector3f::dot(n, wo));
}