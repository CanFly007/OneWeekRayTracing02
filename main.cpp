#include "rtweekend.h"

#include "hittable_list.h"
#include "bvh.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <iostream>

#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

double hit_sphere(const vec3& center, double radius, const ray& r)//返回直线系数t（最小那个最近），或者-1没碰到
{
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = 2.0 * dot(r.direction(), oc);
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return -1.0;
    else
        return (-b - sqrt(discriminant)) / (2.0 * a);
}

hittable_list random_scene()
{
	hittable_list world;
	world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5, make_shared<lambertian>(vec3(0.7, 0.3, 0.3))));
	world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100, make_shared<lambertian>(vec3(0.8, 0.8, 0.0))));

	world.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.5)));//0表示全吸收，所以0.2表示这个球吸收了很多蓝色，用(0.8,0.6,0.2)黄色颜色乘以后面的rayColor,
	world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.8, 0.8), 0.5)));
    //hittable_list构造函数中，把bvh_node*变成hittable*，只add一个bvh_node*元素到objects中
	return static_cast<hittable_list>(make_shared<bvh_node>(world));
	return world;
}
hittable_list earth()
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	
	//Test:
	image_texture imaTex = image_texture(data, width, height);
	for (int j = 100 - 1; j >= 0; --j) //从上往下
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < 200; ++i)
		{
			double u = i / 200.0;
			double v = j / 100.0;
			vec3 color = imaTex.value(u, v);
			color.write_color(std::cout);
		}
	}
	//stbi_image_free(data);
	
	hittable_list world;
	return world;
}

vec3 ray_color(const ray& r, const hittable_list& world,int depth)
{
    if (depth <= 0)
        return vec3(0, 0, 0);

    hit_record rec;//sphere中计算的返回值全在这
    if (world.hit(r, 0.001, infinity, rec))//world中每个物体都会和这条ray判断，在hittable_list会及时更新t_max，以打到最近的hittable
    {
		ray scattered;
		vec3 attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))//碰到的物体，发出scattered射线，变暗了多少atten
			return attenuation * ray_color(scattered, world, depth - 1);
		else
			return vec3(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() 
{
    const int image_width = 200;
    const int image_height = 100;
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);//下面四项是世界坐标
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

	hittable_list world = earth();
	return 1;
	const auto aspect = double(image_width) / image_height;
	vec3 lookfrom(-2, 2, 1);
	vec3 lookat(0, 0, -1);
	vec3 vup(0, 1, 0);//首先用虚拟的Y轴
	double fov = 90;
	camera cam(lookfrom, lookat, vup, fov, aspect);

    for (int j = image_height - 1; j >= 0; --j) //从上往下
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) 
        {
            vec3 color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / image_width;//random_double()在一个像素内的随机
                auto v = (j + random_double()) / image_height;//v的值从上到下1到0
                ray r = cam.get_ray(u, v);
                color += ray_color(r, world, max_depth);//此时color是爆表颜色
            }
            color /= samples_per_pixel;
            auto r = sqrt(color.e[0]);//加sqrt是gamma矫正
            auto g = sqrt(color.e[1]);
            auto b = sqrt(color.e[2]);
            r = clamp(r, 0.0, 0.99);
            g = clamp(g, 0.0, 0.99);
            b = clamp(b, 0.0, 0.99);
            color = vec3(r, g, b);
            color.write_color(std::cout);
        }
    }
    std::cerr << "\nDone.\n";
}