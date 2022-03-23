#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record 
{
    vec3 p;//碰撞的点
    vec3 normal;//碰撞的normal
    double t;//那个点在ray直线上的系数t
};

//任何可能与光线求交的东西实现时都继承这个类
class hittable
{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
#endif // !HITTABLE_H
