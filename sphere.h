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
    //���uv��������������ϵ uΪˮƽphi�Ƕȣ�v����ֱ����theta�Ƕȣ�Ȼ�󽫽Ƕȷ�Χ��һ����[0,1]
    static void get_sphere_uv(const vec3& p, double& u, double& v)
    {
        double phi = atan2(p.z(), p.x());//[-pi, pi] phi����x������ļнǣ��ҳ���z��������Ϊ��ֵ��ֻ�������ѿ�������ϵת��ʱ�������ǶԵ�
        double theta = asin(p.y());//�������ʽ�����ڵ�λԲ�ϣ�theta��Χ��[-pi/2, pi/2]
        
        u = (phi + pi) / (2 * pi);//��������ϵ��z����������pi/2����Ӧu��+0.75�����Դ������¿��Ǵ�x���Ὺʼ˳ʱ��һȦ
        //u = 1 - u;//�������¿�����x���Ὺʼ��ʱ��һȦ
        v = (theta + pi / 2) / pi;//��theta��pi/2����Ӧv��1�����v��Ӧ0����1��0
    }
};

//hittable_list���������hit����
bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const//��main�����ᵽ�����ˣ����ص�ֵȫ��rec��
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
            rec.normal = dot(r.direction(), rec.normal) < 0 ? rec.normal : (-rec.normal);//���ߴ��ڲ����룬����Ҳ���ڲ�
			rec.mat_ptr = mat_ptr;//��ײ�ĵ��¼�Ĳ��� Ϊ��ǰ�����Ĳ���
            //hit�����sphere�󣬻��ø���hit��point���hit���ĸ�uvֵ��
            get_sphere_uv(rec.p, rec.u, rec.v);//rec.u��rec.v�Ǻ�����out���ֵ
            return true;
        }
        t = (-b + sqrt(discriminant)) / (2.0 * a);//��Ϊ���ĵ㲻�ڹ涨���䣬��Զ�ĵ㳢��
        if (t< tmax && t>tmin)
        {
            rec.t = t;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.normal = dot(r.direction(), rec.normal) < 0 ? rec.normal : (-rec.normal);//���ߴ��ڲ����룬����Ҳ���ڲ�
			rec.mat_ptr = mat_ptr;//��ײ�ĵ��¼�Ĳ��� Ϊ��ǰ�����Ĳ���
            get_sphere_uv(rec.p, rec.u, rec.v);
            return true;
        }
    }
    return false;
}

bool sphere::bounding_box(aabb& output_box)const
{
    output_box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
    //���а�Χ�з���true
    return true;
}
#endif