#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"
#include "rtweekend.h"

class material
{
public:
	//1、生成散射后的ray 2、吸收后变暗了多少atten
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const = 0;

	//不是纯虚函数，不用每个材质都实现emitted函数
	virtual vec3 emitted(double u, double v)const
	{
		return vec3(0, 0, 0);
	}
};

class lambertian : public material
{
public:
	lambertian(shared_ptr<texture> a) :albedo(a) {}

	//hit中 之后的，此时rec已经有值，只是算atten和scattered ray
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		vec3 target = record.normal + random_unit_vector();//兰伯特反射点
		scattered = ray(record.p, target);
		atten = albedo->value(record.u, record.v);//vec3构建函数时：这个兰伯特材质时就初始化好了，所以直接赋值返回输出形参
												  //纹理构造函数时：record.u和v是hit到sphere时，已经计算好了，scatter函数在hit成功后计算
		return true;
	}

public:
	shared_ptr<texture> albedo;
};

class metal :public material
{
public:
	metal(const vec3& a, double f) :albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), record.normal);
		reflected += fuzz * random_in_unit_sphere();//fuzz为金属材质球初始化时已赋值属性，偏移一点反射会模糊  0表示清晰 1表示最模糊
		scattered = ray(record.p, reflected);
		atten = albedo;
		return (dot(scattered.direction(), record.normal) > 0);//从normal下面半球射入，反射的方向也是下半球，点积为负，光线不继续追踪
	}

public:
	vec3 albedo;//决定变暗程度 atten 1表示不变暗，0表示全部吸收
	double fuzz;//反射模糊，在反射向量上做一个随机偏移
};

//自发光材质,接收constant_texture作为构造函数
class diffuse_light :public material
{
public:
	shared_ptr<texture> emit;

public:
	diffuse_light(shared_ptr<texture> a) :emit(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		return false;//光线ray碰到这个自发光灯，就不再散射光线出去了
	}
	virtual vec3 emitted(double u, double v)const
	{
		return emit->value(u, v);
	}
};
#endif