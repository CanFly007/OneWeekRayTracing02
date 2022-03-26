#include "rtweekend.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include <iostream>

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

vec3 ray_color(const ray& r, const hittable_list& world)
{
    hit_record rec;//sphere中计算的返回值全在这
    if (world.hit(r, 0, infinity, rec))//world中每个物体都会和这条ray判断，在hittable_list会及时更新t_max，以打到最近的hittable
    {
        return (rec.normal + 1.0) * 0.5;
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

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);//下面四项是世界坐标
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    hittable_list world;
    world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));
    camera cam;

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
                color += ray_color(r, world);//此时color是爆表颜色
            }
            color /= samples_per_pixel;
            auto r = clamp(color.e[0], 0.0, 0.99);
            auto g = clamp(color.e[1], 0.0, 0.99);
            auto b = clamp(color.e[2], 0.0, 0.99);
            color = vec3(r, g, b);
            color.write_color(std::cout);
        }
    }
    std::cerr << "\nDone.\n";
}