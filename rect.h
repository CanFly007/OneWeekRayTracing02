#ifndef RECT_H
#define RECT_H

#include "rtweekend.h"

class xy_rect :public hittable
{
public:
	shared_ptr<material> mp;
	double x0, x1, y0, y1, k;

public:
	xy_rect() {}
	xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> mat)
		:x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec)const;

	//因为实现简单，语法上不用去下面，直接在这里就实现了,而hit在下面实现
	virtual bool bounding_box(double t0, double t1, aabb& output_box)const
	{
		output_box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
		return true;
	}
};

bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec)const
{

}
#endif // !RECT_H
