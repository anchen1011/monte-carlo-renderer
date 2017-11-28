#include "Object3D.h"

bool Sphere::intersect(const Ray &r, float tmin, Hit &h) const
{
    // BEGIN STARTER

    // We provide sphere intersection code for you.
    // You should model other intersection implementations after this one.

    // Locate intersection point ( 2 pts )
    const Vector3f &rayOrigin = r.getOrigin(); //Ray origin in the world coordinate
    const Vector3f &dir = r.getDirection();

    Vector3f origin = rayOrigin - _center;      //Ray origin in the sphere coordinate

    float a = dir.absSquared();
    float b = 2 * Vector3f::dot(dir, origin);
    float c = origin.absSquared() - _radius * _radius;

    // no intersection
    if (b * b - 4 * a * c < 0) {
        return false;
    }

    float d = sqrt(b * b - 4 * a * c);

    float tplus = (-b + d) / (2.0f*a);
    float tminus = (-b - d) / (2.0f*a);

    // the two intersections are at the camera back
    if ((tplus < tmin) && (tminus < tmin)) {
        return false;
    }

    float t = 10000;
    // the two intersections are at the camera front
    if (tminus > tmin) {
        t = tminus;
    }

    // one intersection at the front. one at the back 
    if ((tplus > tmin) && (tminus < tmin)) {
        t = tplus;
    }

    if (t < h.getT()) {
        Vector3f normal = r.pointAtParameter(t) - _center;
        normal = normal.normalized();
        h.set(t, this->material, normal);
        return true;
    }
    // END STARTER
    return false;
}

// Add object to group
void Group::addObject(Object3D *obj) {
    m_members.push_back(obj);
}

// Return number of objects in group
int Group::getGroupSize() const {
    return (int)m_members.size();
}

bool Group::intersect(const Ray &r, float tmin, Hit &h) const
{
    // BEGIN STARTER
    // we implemented this for you
    bool hit = false;
    for (Object3D* o : m_members) {
        if (o->intersect(r, tmin, h)) {
            hit = true;
        }
    }
    return hit;
    // END STARTER
}


Plane::Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
    _normal = normal.normalized();
    _p0 = d * normal.normalized();
}
bool Plane::intersect(const Ray &r, float tmin, Hit &h) const
{
    auto l0 = r.getOrigin();
    auto l = r.getDirection();
    auto n = _normal.normalized();
    float denom = Vector3f::dot(n, l);
    if ( fabsf(denom) > 1e-6) {
        auto t = Vector3f::dot(_p0-l0, n) / denom;
        if (t >= tmin && t < h.getT()) {
            h.set(t,material,_normal);
            return true;
        }
    }
    return false;
}
bool Triangle::intersect(const Ray &r, float tmin, Hit &h) const 
{
    auto a = Matrix3f(_v[0]-_v[1],
                      _v[0]-_v[2],
                      r.getDirection());
    auto b = Vector3f(_v[0]-r.getOrigin());
    Vector3f x = a.inverse() * b;
    if (x[0] < 0 or x[1] < 0 or (x[0] + x[1]) > 1)
        return false;
    auto t = x[2];
    auto normal = (1-x[0]-x[1]) * _normals[0];
    normal += x[0] * _normals[1];
    normal += x[1] * _normals[2];
    if (t >= tmin && t < h.getT()) {
        h.set(t,material,normal.normalized());
        return true;
    }
    return false;
}


Transform::Transform(const Matrix4f &m,
    Object3D *obj) : _object(obj) {
    _m = m;
}
bool Transform::intersect(const Ray &r, float tmin, Hit &h) const
{
    auto im = _m.inverse();
    auto r_o = Ray((im * Vector4f(r.getOrigin(),1)).xyz(), (im * Vector4f(r.getDirection(),0)).xyz());
    h.set(h.getT(),h.getMaterial(),(im.transposed().inverse() * Vector4f(h.getNormal(),0)).xyz().normalized());
    bool ret = _object->intersect(r_o, tmin, h);
    h.set(h.getT(),h.getMaterial(),(im.transposed() * Vector4f(h.getNormal(),0)).xyz().normalized());
    return ret;
}