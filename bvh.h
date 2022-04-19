#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node :public hittable
{
public:
	shared_ptr<hittable> left;//一个node的左右节点是hittable类型，所以可以是另一个bvh_node，也可以是sphere或者其他图元
	shared_ptr<hittable> right;

public:
	bvh_node();
	bvh_node(hittable_list& list) :bvh_node() {}
	bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end);
};

#endif