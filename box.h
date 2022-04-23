#ifndef BOX_H
#define BOX_H

#include "rtweekend.h"
#include "rect.h"
#include "hittable_list.h"

class box :public hittable
{
public:
	vec3 box_min;
	vec3 box_max;
	hittable_list sides;

public:
	box() {}
	box(const vec3& p0, const vec3& p1, shared_ptr<material> mat);

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	virtual bool bounding_box(aabb& output_box) const
	{
		output_box = aabb(box_min, box_max);
		return true;
	}
};

box::box(const vec3& p0, const vec3& p1, shared_ptr<material> mat)
{
	box_min = p0;
	box_max = p1;

	sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
	sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat));

	sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
	sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat));

	sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
	sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat));
}

//立方体有没有被ray hit，就看立方体六个矩形rect有没有被hit
bool box::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
	return sides.hit(r, t_min, t_max, rec);
}
#endif