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
bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const//t_min t_max参数表示这条光线起点和终点，只有一段
{
	hit_record tmp_record;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (const shared_ptr<hittable>& object : objects)
	{
		if (object->hit(r, t_min, closest_so_far, tmp_record))//因为closest_so_far更新，所以只会打到更近的才会更新
		{
			hit_anything = true;
			closest_so_far = tmp_record.t;//射线打到一个物体，此时将max设置为打到物体的，如果再打到更近的才会更新
			rec = tmp_record;
		}
	}
	return hit_anything;
}

bool hittable_list::bounding_box(aabb& output_box) const
{
	if (objects.empty())//整个世界为空，则没有包围盒返回false
		return false;

	aabb temp_box;
	bool first_box = true;

	for (const shared_ptr<hittable> object : objects)
	{
		if (!object->bounding_box(temp_box))//世界中有一个没有包围盒，则世界没法生成包围盒
			return false;
		//否则我们会得到这个object的temp_box信息
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}
	return true;
}

#endif