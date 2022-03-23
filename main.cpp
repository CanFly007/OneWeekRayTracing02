#include "vec3.h"
#include "ray.h"
#include <iostream>

double hit_sphere(const vec3& center, double radius, const ray& r)//����ֱ��ϵ��t����С�Ǹ������������-1û����
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

vec3 ray_color(const ray& r)
{
    double t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if (t > 0.0)//t��ֱ�ߵ�ϵ��
    {
        vec3 normal = unit_vector(r.at(t) - vec3(0, 0, -1));
        return (normal + 1.0) * 0.5;
    }

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() 
{
    const int image_width = 200;
    const int image_height = 100;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    for (int j = image_height - 1; j >= 0; --j) //��������
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) 
        {
            auto u = double(i) / image_width;
            auto v = double(j) / image_height;//���ϵ���1��0
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 color = ray_color(r);
            color.write_color(std::cout);
        }
    }
    std::cerr << "\nDone.\n";
}