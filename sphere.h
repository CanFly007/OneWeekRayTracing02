#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
    sphere() {}
    sphere(vec3 cen, double r) : center(cen), radius(r) {};

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;

public:
    vec3 center;
    double radius;
};

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const
{
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = 2.0 * dot(r.direction(), oc);
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant > 0)
    {
        double t = (-b - sqrt(discriminant)) / (2.0 * a);
        if (t< tmax && t>tmin)
        {
            rec.t = t;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        t = (-b + sqrt(discriminant)) / (2.0 * a);//因为近的点不在规定区间，用远的点尝试
        if (t< tmax && t>tmin)
        {
            rec.t = t;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
#endif