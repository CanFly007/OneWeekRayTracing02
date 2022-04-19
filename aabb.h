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
		//xyz三个轴，每个轴算出来的tmin和tmax都表示直线上固定一个点位置t，用t做是否重合算法
		for (int axis = 0; axis < 3; axis++)
		{
			double t0 = (_min[axis] - r.origin()[axis]) / r.direction()[axis];//比如X轴上，aabb盒子和射线交点：射线的t0位置
			double t1 = (_max[axis] - r.origin()[axis]) / r.direction()[axis];
			//保持t0一直小于t1
			double smallerT = ffmin(t0, t1);
			double biggerT = ffmax(t0, t1);//smallT和biggerT是射线与aabb盒子X轴的交点 t位置

			//重叠区域算法，可以画重叠，一条在另外一条全左，在另外一条全右，三条线段直观看
			//射线tmin到tmax区段 是否和交点区段有重合区域
			tmin = ffmax(smallerT, tmin);
			tmax = ffmin(biggerT, tmax);
			if (tmax <= tmin)//该线段区间在X维度都没有重叠区域了，就不用看Y和Z维度了
				return false;
			//不返回false，则说明有重叠，ray的tmin和tmax更新为与X维度的重叠部分，再接下来判断更新后的部分在Y和Z维度情况
		}
		return true;
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
#endif