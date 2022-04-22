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
#include "rect.h"

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
    //��Ϊlambertian���й��캯����vec3���texture������ע��
	//world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5, make_shared<lambertian>(vec3(0.7, 0.3, 0.3))));
	//world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100, make_shared<lambertian>(vec3(0.8, 0.8, 0.0))));

	world.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.5)));//0��ʾȫ���գ�����0.2��ʾ����������˺ܶ���ɫ����(0.8,0.6,0.2)��ɫ��ɫ���Ժ����rayColor,
	world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.8, 0.8), 0.5)));
    //hittable_list���캯���У���bvh_node*���hittable*��ֻaddһ��bvh_node*Ԫ�ص�objects��
	return static_cast<hittable_list>(make_shared<bvh_node>(world));
	return world;
}
hittable_list earth()
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load("earthmap.jpg", &width, &height, &nrChannels, 0);
	
    shared_ptr<image_texture> earthTex = make_shared<image_texture>(data, width, height);
    shared_ptr<lambertian> earthMat = make_shared<lambertian>(earthTex);//֮ǰ�������ع��캯���ǽ���vec3��ɫ�����ڽ���һ������
    shared_ptr<sphere> globe = make_shared<sphere>(vec3(0, 0, 0), 2, earthMat);

    return hittable_list(globe);//hittable_list���캯���ٵ���add������objects����һ��Ԫ��->globe
}
hittable_list simple_light()//һյ�ƹ⣬һ������
{
	hittable_list objects;
	shared_ptr<constant_texture> groundTex = make_shared<constant_texture>(vec3(0.8, 0.8, 0.0));
	shared_ptr<lambertian> groundMat = make_shared<lambertian>(groundTex);
	shared_ptr<sphere> ground = make_shared<sphere>(vec3(0, -1000, 0), 1000, groundMat);
	objects.add(ground);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("earthmap.jpg", &width, &height, &nrChannels, 0);
	shared_ptr<image_texture> earthTex = make_shared<image_texture>(data, width, height);
	shared_ptr<lambertian> earthMat = make_shared<lambertian>(earthTex);//֮ǰ�������ع��캯���ǽ���vec3��ɫ�����ڽ���һ������
	shared_ptr<sphere> globe = make_shared<sphere>(vec3(0, 2, 0), 2, earthMat);
	objects.add(globe);

	shared_ptr<constant_texture> lightTex = make_shared<constant_texture>(vec3(4, 4, 4));
	shared_ptr<diffuse_light> lightMat = make_shared<diffuse_light>(lightTex);
	shared_ptr<xy_rect> xyRect = make_shared<xy_rect>(3, 5, 1, 3, -2, lightMat);
	objects.add(xyRect);

	return objects;
}

vec3 ray_color(const ray& r, const hittable_list& world,int depth)
{
    if (depth <= 0)
        return vec3(0, 0, 0);

    hit_record rec;//sphere�м���ķ���ֵȫ����
    if (world.hit(r, 0.001, infinity, rec))//world��ÿ�����嶼�������ray�жϣ���hittable_list�ἰʱ����t_max���Դ������hittable
    {
		ray scattered;
		vec3 attenuation;//atten�������Ϊ�����ײ����ɫֵ�����ռ��뵽ray_color�ĵݹ����������������albedo        
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))//���������壬����scattered���ߣ��䰵�˶���atten
            return attenuation * ray_color(scattered, world, depth - 1);
		else//���������ڲ�������Է�����ʶ�scatter����false
		{
			vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v);//�Է�����ʵ���ɫ
			return emitted;//���Է�����ʣ��������ȡ���򵽽����ڲ�������000
		}
    }

    //ͨ������hittable_list������rayû��world���κζ��������ر���ɫ��֮ǰ����ɫ������Ϊ��ɫ
    return vec3(0, 0, 0);

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

    hittable_list world;

    const auto aspect = double(image_width) / image_height;
    vec3 lookfrom;
    vec3 lookat;
    vec3 vup(0, 1, 0);//�����������Y��
    double fov;

    switch (1)
    {
    case 0:
        world = earth();
        lookfrom = vec3(0, 0, 12);
        lookat = vec3(0, 0, 0);
        fov = 20;
	case 1:
		world = simple_light();
		lookfrom = vec3(26, 3, 6);
		lookat = vec3(0, 2, 0);
		fov = 20;
    default:
        break;
    }

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