#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list :hittable
{
public:
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(aabb& output_box) const override;

public:
	std::vector<shared_ptr<hittable>> objects;
};
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const//t_min t_max������ʾ�������������յ㣬ֻ��һ��
{
	hit_record tmp_record;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (const shared_ptr<hittable>& object : objects)
	{
		if (object->hit(r, t_min, closest_so_far, tmp_record))//��Ϊclosest_so_far���£�����ֻ��򵽸����ĲŻ����
		{
			hit_anything = true;
			closest_so_far = tmp_record.t;//���ߴ�һ�����壬��ʱ��max����Ϊ������ģ�����ٴ򵽸����ĲŻ����
			rec = tmp_record;
		}
	}
	return hit_anything;
}

bool hittable_list::bounding_box(aabb& output_box) const
{
	if (objects.empty())//��������Ϊ�գ���û�а�Χ�з���false
		return false;

	aabb temp_box;
	bool first_box = true;

	for (const shared_ptr<hittable> object : objects)
	{
		if (!object->bounding_box(temp_box))//��������һ��û�а�Χ�У�������û�����ɰ�Χ��
			return false;
		//�������ǻ�õ����object��temp_box��Ϣ
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}
	return true;
}

#endif