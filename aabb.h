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

	//注意看不是覆写hittable中的hit方法，少了一个参数，参数tmin和tmax表示ray的区间
	//判断ray和这个aabb盒子是否相交
	bool hit(const ray& r, double tmin, double tmax)const
	{
		//xyz三个轴，每个轴算出来的tmin和tmax都表示直线上固定一个点位置
		for (int axis = 0; axis < 3; axis++)
		{
			double t0 = (_min[axis] - r.origin()[axis]) / r.direction()[axis];//比如X轴上，aabb盒子和射线交点：射线的t0位置
			double t1 = (_max[axis] - r.origin()[axis]) / r.direction()[axis];
			//保持t0一直小于t1
			double smaller = ffmin(t0, t1);
		}
	}
};

//全局工具方法：合并box0和box1返回大的盒子
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