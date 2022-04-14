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
		w = unit_vector(lookfrom - lookat);//���z��������������ϵ
		u = unit_vector(cross(vup, w));//�ü�vupȥ������õ����x����
		v = cross(w, u);//w��u���Ѿ���һ��

		lower_left_corner = origin - half_width * u - half_height * v - w;//-w��ʾ��z�ĸ����ᣬ��������ϵ
		horizontal = 2 * half_width * u;
		vertical = 2 * half_height * v;
    }

    ray get_ray(double u, double v)//u��v�ķ�Χ��[0,1]��ray������������ 
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