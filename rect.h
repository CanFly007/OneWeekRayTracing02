#ifndef RECT_H
#define RECT_H

#include "rtweekend.h"
#include "hittable.h"

class xy_rect :public hittable
{
public:
	shared_ptr<material> mat;
	double x0, x1, y0, y1, k;

public:
	xy_rect() {}
	xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> m)
		:x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;

	//��Ϊʵ�ּ򵥣��﷨�ϲ���ȥ���棬ֱ���������ʵ����,��hit������ʵ��
	virtual bool bounding_box(aabb& output_box)const
	{
		output_box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
		return true;
	}
};
class xz_rect :public hittable
{
public:
	shared_ptr<material> mat;
	double x0, x1, z0, z1, k;

public:
	xz_rect() {}
	xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> m)
		:x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;

	//��Ϊʵ�ּ򵥣��﷨�ϲ���ȥ���棬ֱ���������ʵ����,��hit������ʵ��
	virtual bool bounding_box(aabb& output_box)const
	{
		output_box = aabb(vec3(x0, k - 0.0001, z0), vec3(x1, k + 0.0001, z1));
		return true;
	}
};
class yz_rect :public hittable
{
public:
	shared_ptr<material> mat;
	double y0, y1, z0, z1, k;

public:
	yz_rect() {}
	yz_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<material> m)
		:y0(_x0), y1(_x1), z0(_z0), z1(_z1), k(_k), mat(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;

	//��Ϊʵ�ּ򵥣��﷨�ϲ���ȥ���棬ֱ���������ʵ����,��hit������ʵ��
	virtual bool bounding_box(aabb& output_box)const
	{
		output_box = aabb(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
		return true;
	}
};

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	//1.��Ϊ������һ�Σ���tmin��tmax�����жϹ�����һ�����Ƿ�����
	double t = (k - r.origin().z()) / r.direction().z();//xyƽ�棬k������������zά�ȵ���kʱ��t���Ķ�
	if (t<t_min || t>t_max) //ֱ��zά�ȵ�kλ��ʱ��t�Ѿ������������������ˣ���Ȼ���ཻ
		return false;
	//2.��������һ���Σ�����xyƽ���ཻ�ĵ㣬�Ƿ��ھ�����
	double x = r.origin().x() + t * r.direction().x();//xά��
	double y = r.origin().y() + t * r.direction().y();
	if (x<x0 || x>x1 || y<y0 || y>y1)
		return false;
	//3.֤������ray hit�����rect����
	rec.t = t;
	rec.p = vec3(x, y, k);//Ҳ���Ա�ʾ�� r.at(t)
	rec.normal = dot(vec3(0, 0, 1), r.direction()) < 0 ? vec3(0, 0, 1) : vec3(0, 0, -1);
	rec.mat_ptr = mat;//��ײ��rect��ʹ��rect�Ĳ��ʣ�����ʱ���Ѿ���ֵ��mat����
	rec.u = (x - x0) / (x1 - x0);//[0,1]
	rec.v = (y - y0) / (y1 - y0);
	return true;
}
bool xz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	double t = (k - r.origin().y()) / r.direction().y();
	if (t<t_min || t>t_max)
		return false;

	double x = r.origin().x() + t * r.direction().x();
	double z = r.origin().z() + t * r.direction().z();
	if (x<x0 || x>x1 || z<z0 || z>z1)
		return false;

	rec.t = t;
	rec.p = vec3(x, k, z);
	rec.normal = dot(vec3(0, 1, 0), r.direction()) < 0 ? vec3(0, 1, 0) : vec3(0, -1, 0);
	rec.mat_ptr = mat;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	return true;
}
bool yz_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{
	double t = (k - r.origin().x()) / r.direction().x();
	if (t<t_min || t>t_max)
		return false;

	double y = r.origin().y() + t * r.direction().y();
	double z = r.origin().z() + t * r.direction().z();
	if (y<y0 || y>y1 || z<z0 || z>z1)
		return false;

	rec.t = t;
	rec.p = vec3(k, y, z);
	rec.normal = dot(vec3(1, 0, 0), r.direction()) < 0 ? vec3(1, 0, 0) : vec3(-1, 0, 0);
	rec.mat_ptr = mat;
	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	return true;
}
#endif // !RECT_H
