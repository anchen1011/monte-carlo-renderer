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
    return 1.0f;
}