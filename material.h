#ifndef MATERIAL_H
#define MATERIAL_H


class material
{
	//1������ɢ����ray 2�����պ�䰵�˶���atten
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const = 0;
};
#endif