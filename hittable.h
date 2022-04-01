#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class material;

struct hit_record 
{
    vec3 p;//��ײ�ĵ�
    vec3 normal;//��ײ��normal
    double t;//�Ǹ�����rayֱ���ϵ�ϵ��t
	shared_ptr<material> mat_ptr;//�����Ǹ���Ĳ��������1��ô��ɢ��2��ɢ��ray˥�����٣�
};

//�κο���������󽻵Ķ���ʵ��ʱ���̳������
class hittable
{
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
#endif // !HITTABLE_H
