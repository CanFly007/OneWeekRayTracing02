#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material
{
public:
	//1、生成散射后的ray 2、吸收后变暗了多少atten
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const = 0;
};

class lambertian : public material
{
public:
	lambertian(const vec3& a) :albedo(a) {}

	//hit中 之后的，此时rec已经有值，只是算atten和scattered ray
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		vec3 target = record.normal + random_unit_vector();//兰伯特反射点
		scattered = ray(record.p, target);
		atten = albedo;//构建这个兰伯特材质时就初始化好了，所以直接赋值返回输出形参
		return true;
	}

public:
	vec3 albedo;
};
#endif