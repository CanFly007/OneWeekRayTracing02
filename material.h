#ifndef MATERIAL_H
#define MATERIAL_H


class material
{
	//1、生成散射后的ray 2、吸收后变暗了多少atten
	virtual bool scatter(const ray& r_in, const hit_record& record, vec3& atten, ray& scattered)const = 0;
};
#endif