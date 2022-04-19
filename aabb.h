#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb
{
public:
	vec3 _min;
	vec3 _max;

	aabb() {}
	aabb(const vec3& a, const vec3& b) { _min = a; _max = b; }

	vec3 min() const { return _min; }
	vec3 max() const { return _max; }

	//ע�⿴���Ǹ�дhittable�е�hit����������һ������������tmin��tmax��ʾray������
	//�ж�ray�����aabb�����Ƿ��ཻ
	bool hit(const ray& r, double tmin, double tmax)const
	{
		//xyz�����ᣬÿ�����������tmin��tmax����ʾֱ���Ϲ̶�һ����λ��t����t���Ƿ��غ��㷨
		for (int axis = 0; axis < 3; axis++)
		{
			double t0 = (_min[axis] - r.origin()[axis]) / r.direction()[axis];//����X���ϣ�aabb���Ӻ����߽��㣺���ߵ�t0λ��
			double t1 = (_max[axis] - r.origin()[axis]) / r.direction()[axis];
			//����t0һֱС��t1
			double smallerT = ffmin(t0, t1);
			double biggerT = ffmax(t0, t1);//smallT��biggerT��������aabb����X��Ľ��� tλ��

			//�ص������㷨�����Ի��ص���һ��������һ��ȫ��������һ��ȫ�ң������߶�ֱ�ۿ�
			//����tmin��tmax���� �Ƿ�ͽ����������غ�����
			tmin = ffmax(smallerT, tmin);
			tmax = ffmin(biggerT, tmax);
			if (tmax <= tmin)//���߶�������Xά�ȶ�û���ص������ˣ��Ͳ��ÿ�Y��Zά����
				return false;
			//������false����˵�����ص���ray��tmin��tmax����Ϊ��Xά�ȵ��ص����֣��ٽ������жϸ��º�Ĳ�����Y��Zά�����
		}
		return true;
	}
};

//ȫ�ֹ��߷������ϲ�box0��box1���ش�ĺ���
aabb surrounding_box(aabb box0, aabb box1)
{
	vec3 small(ffmin(box0.min().x(), box1.min().x()),
		ffmin(box0.min().y(), box1.min().y()),
		ffmin(box0.min().z(), box1.min().z()));

	vec3 big(ffmax(box0.max().x(), box1.max().x()),
		ffmax(box0.max().y(), box1.max().y()),
		ffmax(box0.max().z(), box1.max().z()));

	return aabb(small, big);
}
#elif