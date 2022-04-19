#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node :public hittable
{
public:
	shared_ptr<hittable> left;//һ��node�����ҽڵ���hittable*���ͣ����Կ�������һ��bvh_node��Ҳ������sphere��������ͼԪ
	shared_ptr<hittable> right;
	aabb box;//���node��AABB����

public:
	bvh_node();
	bvh_node(hittable_list& list) :bvh_node() {}
	bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end);

	virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
	virtual bool bounding_box(aabb& output_box) const override;
};

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