#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node :public hittable
{
public:
	shared_ptr<hittable> left;//一个node的左右节点是hittable*类型，所以可以是另一个bvh_node，也可以是sphere或者其他图元
	shared_ptr<hittable> right;
	aabb box;//这个node的AABB盒子

public:
	bvh_node();
	bvh_node(hittable_list& list) :bvh_node() {}
	bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end);

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(aabb& output_box) const override;
};

bool bvh_node::hit(const ray& r, double tmin, double tmax, hit_record& rec) const
{
	//用了aabb里的hit
	if (!box.hit(r, tmin, tmax))//box为这个节点已经赋值的盒子，判断ray和这个aabb是否相交
		return false;

	//用了hittable里的hit
	bool hit_left = left->hit(r, tmin, tmax, rec);//left如果还是bvh_node，则递归这个函数且先用box判断是否相交
	bool hit_right = right->hit(r, tmin, hit_left ? rec.t : tmax, rec); //left如果是sphere等真的物体，则用sphere里的hit
	return hit_left || hit_right;
}

bool bvh_node::bounding_box(aabb& output_box) const
{
	output_box = box;//box为类对象，在这个bvh节点的构造函数中已经赋值，所以这里直接传递出去即可
	return true;
}

#endif