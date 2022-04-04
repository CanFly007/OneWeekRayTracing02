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

class metal :public material
{
public:
	metal(const vec3& a) :albedo(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), record.normal);
		scattered = ray(record.p, reflected);
		atten = albedo;
		return (dot(scattered.direction(), record.normal) > 0);//��normal����������룬����ķ���Ҳ���°��򣬵��Ϊ�������߲�����׷��
	}

public:
	vec3 albedo;//�����䰵�̶� atten 1��ʾ���䰵��0��ʾȫ������
};
#endif