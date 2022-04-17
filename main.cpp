#include "rtweekend.h"

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
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

hittable_list random_scene()
{
	hittable_list world;
	world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5, make_shared<lambertian>(vec3(0.7, 0.3, 0.3))));
	world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100, make_shared<lambertian>(vec3(0.8, 0.8, 0.0))));

	world.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.5)));//0��ʾȫ���գ�����0.2��ʾ����������˺ܶ���ɫ����(0.8,0.6,0.2)��ɫ��ɫ���Ժ����rayColor,
	world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.8, 0.8), 0.5)));

	return world;
}

vec3 ray_color(const ray& r, const hittable_list& world,int depth)
{
    if (depth <= 0)
        return vec3(0, 0, 0);

    hit_record rec;//sphere�м���ķ���ֵȫ����
    if (world.hit(r, 0.001, infinity, rec))//world��ÿ�����嶼�������ray�жϣ���hittable_list�ἰʱ����t_max���Դ������hittable
    {
		ray scattered;
		vec3 attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))//���������壬����scattered���ߣ��䰵�˶���atten
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
    vec3 lower_left_corner(-2.0, -1.0, -1.0);//������������������
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

	hittable_list world = random_scene();

	const auto aspect = double(image_width) / image_height;
	vec3 lookfrom(-2, 2, 1);
	vec3 lookat(0, 0, -1);
	vec3 vup(0, 1, 0);//�����������Y��
	double fov = 90;
	camera cam(lookfrom, lookat, vup, fov, aspect);

    for (int j = image_height - 1; j >= 0; --j) //��������
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) 
        {
            vec3 color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / image_width;//random_double()��һ�������ڵ����
                auto v = (j + random_double()) / image_height;//v��ֵ���ϵ���1��0
                ray r = cam.get_ray(u, v);
                color += ray_color(r, world, max_depth);//��ʱcolor�Ǳ�����ɫ
            }
            color /= samples_per_pixel;
            auto r = sqrt(color.e[0]);//��sqrt��gamma����
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