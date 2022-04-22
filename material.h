#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"
#include "rtweekend.h"

class material
{
public:
	//1������ɢ����ray 2�����պ�䰵�˶���atten
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const = 0;

	//���Ǵ��麯��������ÿ�����ʶ�ʵ��emitted����
	virtual vec3 emitted(double u, double v)const
	{
		return vec3(0, 0, 0);
	}
};

class lambertian : public material
{
public:
	lambertian(shared_ptr<texture> a) :albedo(a) {}

	//hit�� ֮��ģ���ʱrec�Ѿ���ֵ��ֻ����atten��scattered ray
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		vec3 target = record.normal + random_unit_vector();//�����ط����
		scattered = ray(record.p, target);
		atten = albedo->value(record.u, record.v);//vec3��������ʱ����������ز���ʱ�ͳ�ʼ�����ˣ�����ֱ�Ӹ�ֵ��������β�
												  //�����캯��ʱ��record.u��v��hit��sphereʱ���Ѿ�������ˣ�scatter������hit�ɹ������
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
		reflected += fuzz * random_in_unit_sphere();//fuzzΪ�����������ʼ��ʱ�Ѹ�ֵ���ԣ�ƫ��һ�㷴���ģ��  0��ʾ���� 1��ʾ��ģ��
		scattered = ray(record.p, reflected);
		atten = albedo;
		return (dot(scattered.direction(), record.normal) > 0);//��normal����������룬����ķ���Ҳ���°��򣬵��Ϊ�������߲�����׷��
	}

public:
	vec3 albedo;//�����䰵�̶� atten 1��ʾ���䰵��0��ʾȫ������
	double fuzz;//����ģ�����ڷ�����������һ�����ƫ��
};

//�Է������,����constant_texture��Ϊ���캯��
class diffuse_light :public material
{
public:
	shared_ptr<texture> emit;

public:
	diffuse_light(shared_ptr<texture> a) :emit(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const
	{
		return false;//����ray��������Է���ƣ��Ͳ���ɢ����߳�ȥ��
	}
	virtual vec3 emitted(double u, double v)const
	{
		return emit->value(u, v);
	}
};
#endif