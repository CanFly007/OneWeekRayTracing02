#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

class bvh_node :public hittable
{
public:
	shared_ptr<hittable> left;//一个node的左右节点是hittable*类型，所以可以是另一个bvh_node，也可以是sphere或者其他图元
	shared_ptr<hittable> right;
	aabb box;//这个node的AABB盒子

public:
	bvh_node();
	bvh_node(hittable_list& list) :bvh_node(list.objects, 0, list.objects.size()) {}
	bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end);

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(aabb& output_box) const override;
};

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis)
{
	aabb box_a;
	aabb box_b;
	if (!a->bounding_box(box_a) || !b->bounding_box(box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.min().e[axis] < box_b.min().e[axis];
}
bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
	return box_compare(a, b, 0);
}
bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
	return box_compare(a, b, 1);
}
bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
{
	return box_compare(a, b, 2);
}

//main中调用	return static_cast<hittable_list>(make_shared<bvh_node>(world));
bvh_node::bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end)
{
	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? box_x_compare
					: (axis == 1) ? box_y_compare
								  : box_z_compare;

	size_t object_span = end - start;
	if (object_span == 1)
	{
		left = right = objects[start];//左右节点都是物体，非bvh_node
	}
	else if (object_span == 2)//左右节点都是真实的物体，非bvh_node
	{
		if (comparator(objects[start], objects[start + 1]))
		{
			left = objects[start];
			right = objects[start + 1];
		}
		else
		{
			left = objects[start + 1];
			right = objects[start];
		}
	}
	else
	{
		std::sort(objects.begin() + start, objects.begin() + end, comparator);

		size_t mid = start + object_span / 2;
		left = make_shared<bvh_node>(objects, start, mid);//左右都是bvh_node，继续递归往下细分
		right = make_shared<bvh_node>(objects, mid, end);
	}
	//上面在构造函数中，构造了一颗bvh树，最底层都是真实物体，上面都是bvh_node节点构成的
	//下面计算当前bvh_node节点的box
	aabb box_left, box_right;
	if(!left->bounding_box(box_left) || !right->bounding_box(box_right))
		std::cerr << "No bounding box in bvh_node constructor.\n";
	box = surrounding_box(box_left, box_right);//最底层是由左右2个真实物体构成的box，赋值给这个bvh_node对象的box变量
}

//hittable_list类调用object->hit，实际上objects里面只有一个bvh_node*元素
//每个bvh_node节点都有aabb盒子，底下的left和right可能也是bvh_node，或者是最底层sphere实物hit函数
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