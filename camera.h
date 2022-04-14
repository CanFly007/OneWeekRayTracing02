#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect)
	{
		origin = lookfrom;

		double theta = degrees_to_radians(vfov);
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;

		vec3 u, v, w;
		w = unit_vector(lookfrom - lookat);//真的z向量，右手坐标系
		u = unit_vector(cross(vup, w));//用假vup去叉积，得到真的x向量
		v = cross(w, u);//w和u都已经归一化

		lower_left_corner = origin - half_width * u - half_height * v - w;//-w表示在z的负数轴，右手坐标系
		horizontal = 2 * half_width * u;
		vertical = 2 * half_height * v;
    }

    ray get_ray(double u, double v)//u和v的范围是[0,1]，ray是在世界坐标 
    {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }

public:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};
#endif