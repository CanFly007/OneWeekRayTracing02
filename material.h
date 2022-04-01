#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material
{
public:
	//1������ɢ����ray 2�����պ�䰵�˶���atten
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const = 0;
};

class lambertian : public material
{
public:
	lambertian(const vec3& a) :albedo(a) {}

	//hit�� ֮��ģ���ʱrec�Ѿ���ֵ��ֻ����atten��scattered ray
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		vec3 target = record.normal + random_unit_vector();//�����ط����
		scattered = ray(record.p, target);
		atten = albedo;//������������ز���ʱ�ͳ�ʼ�����ˣ�����ֱ�Ӹ�ֵ��������β�
		return true;
	}

public:
	vec3 albedo;
};
#endif