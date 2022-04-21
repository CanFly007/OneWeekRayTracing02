#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "aabb.h"

class material;

struct hit_record 
{
    vec3 p;//碰撞的点
    vec3 normal;//碰撞的normal
    double t;//那个点在ray直线上的系数t
	shared_ptr<material> mat_ptr;//碰到那个点的材质情况（1怎么发散，2发散的ray衰减多少）

	double u;
	double v;
};

//任何可能与光线求交的东西实现时都继承这个类
class hittable
{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(aabb& output_box) const = 0;//返回false表示该物体没有包围盒，在子类out输出该物体包围盒
};
#endif // !HITTABLE_H
