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

class metal :public material
{
public:
	metal(const vec3& a) :albedo(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), record.normal);
		scattered = ray(record.p, reflected);
		atten = albedo;
		return (dot(scattered.direction(), record.normal) > 0);//从normal下面半球射入，反射的方向也是下半球，点积为负，光线不继续追踪
	}

public:
	vec3 albedo;//决定变暗程度 atten 1表示不变暗，0表示全部吸收
};
#endif