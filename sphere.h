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

private:
    //球的uv，按照球面坐标系 u为水平phi角度，v是竖直方向theta角度，然后将角度范围归一化到[0,1]

    static void get_sphere_uv(const vec3& p, double& u, double& v)
    {
        double phi = atan2(p.z(), p.x());//[-pi, pi] phi是与x正方向的夹角，且朝向z轴正方向为正值，只有这样笛卡尔坐标系转换时正负才是对的
        double theta = asin(p.y());//按这个公式，是在单位圆上，theta范围是[-pi/2, pi/2]
        
        u = (phi + pi) / (2 * pi);//右手坐标系，z轴正方向是pi/2，对应u是+0.75，所以从上往下看是从x负轴开始顺时针一圈
        //u = 1 - u;//从上往下看，从x负轴开始逆时针一圈
        v = (theta + pi / 2) / pi;//球顶theta是pi/2，对应v是1，球底v对应0。从1到0
    }
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
            //hit到这个sphere后，还得根据hit的point算出hit到哪个uv值上
            get_sphere_uv(rec.p, rec.u, rec.v);//rec.u和rec.v是函数的out输出值
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
            get_sphere_uv(rec.p, rec.u, rec.v);
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