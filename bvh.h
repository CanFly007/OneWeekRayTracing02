#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

class bvh_node :public hittable
{
public:
	shared_ptr<hittable> left;//һ��node�����ҽڵ���hittable*���ͣ����Կ�������һ��bvh_node��Ҳ������sphere��������ͼԪ
	shared_ptr<hittable> right;
	aabb box;//���node��AABB����

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

//main�е���	return static_cast<hittable_list>(make_shared<bvh_node>(world));
bvh_node::bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end)
{
	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? box_x_compare
					: (axis == 1) ? box_y_compare
								  : box_z_compare;

	size_t object_span = end - start;
	if (object_span == 1)
	{
		left = right = objects[start];//���ҽڵ㶼�����壬��bvh_node
	}
	else if (object_span == 2)//���ҽڵ㶼����ʵ�����壬��bvh_node
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
		left = make_shared<bvh_node>(objects, start, mid);//���Ҷ���bvh_node�������ݹ�����ϸ��
		right = make_shared<bvh_node>(objects, mid, end);
	}
	//�����ڹ��캯���У�������һ��bvh������ײ㶼����ʵ���壬���涼��bvh_node�ڵ㹹�ɵ�
	//������㵱ǰbvh_node�ڵ��box
	aabb box_left, box_right;
	if(!left->bounding_box(box_left) || !right->bounding_box(box_right))
		std::cerr << "No bounding box in bvh_node constructor.\n";
	box = surrounding_box(box_left, box_right);//��ײ���������2����ʵ���幹�ɵ�box����ֵ�����bvh_node�����box����
}

bool bvh_node::hit(const ray& r, double tmin, double tmax, hit_record& rec) const
{
	//����aabb���hit
	if (!box.hit(r, tmin, tmax))//boxΪ����ڵ��Ѿ���ֵ�ĺ��ӣ��ж�ray�����aabb�Ƿ��ཻ
		return false;

	//����hittable���hit
	bool hit_left = left->hit(r, tmin, tmax, rec);//left�������bvh_node����ݹ��������������box�ж��Ƿ��ཻ
	bool hit_right = right->hit(r, tmin, hit_left ? rec.t : tmax, rec); //left�����sphere��������壬����sphere���hit
	return hit_left || hit_right;
}

bool bvh_node::bounding_box(aabb& output_box) const
{
	output_box = box;//boxΪ����������bvh�ڵ�Ĺ��캯�����Ѿ���ֵ����������ֱ�Ӵ��ݳ�ȥ����
	return true;
}


#endif