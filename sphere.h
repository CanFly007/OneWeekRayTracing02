#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable
{
public:
    sphere() {}
    sphere(vec3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
    virtual bool bounding_box(aabb& output_box) const override;

public:
    vec3 center;
    double radius;
	shared_ptr<material> mat_ptr;
};

//hittable_list调用球类的hit函数
bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const//从main函数搬到这来了，返回的值全在rec中
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
            rec.normal = dot(r.direction(), rec.normal) < 0 ? rec.normal : (-rec.normal);//射线从内部射入，法线也在内部
			rec.mat_ptr = mat_ptr;//碰撞的点记录的材质 为当前类对象的材质
            return true;
        }
        t = (-b + sqrt(discriminant)) / (2.0 * a);//因为近的点不在规定区间，用远的点尝试
        if (t< tmax && t>tmin)
        {
            rec.t = t;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.normal = dot(r.direction(), rec.normal) < 0 ? rec.normal : (-rec.normal);//射线从内部射入，法线也在内部
			rec.mat_ptr = mat_ptr;//碰撞的点记录的材质 为当前类对象的材质
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(aabb& output_box)const
{
    output_box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    //球有包围盒返回true
    return true;
}
#endif