#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node :public hittable
{
public:
	shared_ptr<hittable> left;//һ��node�����ҽڵ���hittable���ͣ����Կ�������һ��bvh_node��Ҳ������sphere��������ͼԪ
	shared_ptr<hittable> right;

public:
	bvh_node();
	bvh_node(hittable_list& list) :bvh_node() {}
	bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end);
};

#endif