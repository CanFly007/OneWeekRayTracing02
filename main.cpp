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
#include "box.h"

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
    //因为lambertian类中构造函数从vec3变成texture，所以注释
	//world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5, make_shared<lambertian>(vec3(0.7, 0.3, 0.3))));
	//world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100, make_shared<lambertian>(vec3(0.8, 0.8, 0.0))));

	world.add(make_shared<sphere>(vec3(1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.6, 0.2), 0.5)));//0表示全吸收，所以0.2表示这个球吸收了很多蓝色，用(0.8,0.6,0.2)黄色颜色乘以后面的rayColor,
	world.add(make_shared<sphere>(vec3(-1, 0, -1), 0.5, make_shared<metal>(vec3(0.8, 0.8, 0.8), 0.5)));
    //hittable_list构造函数中，把bvh_node*变成hittable*，只add一个bvh_node*元素到objects中
	return static_cast<hittable_list>(make_shared<bvh_node>(world));
	return world;
}
hittable_list earth()
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load("earthmap.jpg", &width, &height, &nrChannels, 0);
	
    shared_ptr<image_texture> earthTex = make_shared<image_texture>(data, width, height);
    shared_ptr<lambertian> earthMat = make_shared<lambertian>(earthTex);//之前的兰伯特构造函数是接受vec3颜色，现在接受一张纹理
    shared_ptr<sphere> globe = make_shared<sphere>(vec3(0, 0, 0), 2, earthMat);

    return hittable_list(globe);//hittable_list构造函数再调用add函数，objects包含一个元素->globe
}
hittable_list simple_light()//一盏灯光，一个地球
{
	hittable_list objects;
	shared_ptr<constant_texture> groundTex = make_shared<constant_texture>(vec3(0.8, 0.8, 0.0));
	shared_ptr<lambertian> groundMat = make_shared<lambertian>(groundTex);
	shared_ptr<sphere> ground = make_shared<sphere>(vec3(0, -1000, 0), 1000, groundMat);
	objects.add(ground);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("earthmap.jpg", &width, &height, &nrChannels, 0);
	shared_ptr<image_texture> earthTex = make_shared<image_texture>(data, width, height);
	shared_ptr<lambertian> earthMat = make_shared<lambertian>(earthTex);//之前的兰伯特构造函数是接受vec3颜色，现在接受一张纹理
	shared_ptr<sphere> globe = make_shared<sphere>(vec3(0, 2, 0), 2, earthMat);
	objects.add(globe);

	shared_ptr<constant_texture> lightTex = make_shared<constant_texture>(vec3(4, 4, 4));
	shared_ptr<diffuse_light> lightMat = make_shared<diffuse_light>(lightTex);
	shared_ptr<xy_rect> xyRect = make_shared<xy_rect>(3, 5, 1, 3, -2, lightMat);
	objects.add(xyRect);

	return objects;
}
hittable_list cornell_box()
{
	hittable_list objects;
	shared_ptr<constant_texture> redTex = make_shared<constant_texture>(vec3(0.65, 0.05, 0.05));
	shared_ptr<constant_texture> whiteTex = make_shared<constant_texture>(vec3(0.73, 0.73, 0.73));
	shared_ptr<constant_texture> greenTex = make_shared<constant_texture>(vec3(0.12, 0.45, 0.15));
	shared_ptr<constant_texture> lightTex = make_shared<constant_texture>(vec3(15, 15, 15));

	shared_ptr<lambertian> redMat = make_shared<lambertian>(redTex);
	shared_ptr<lambertian> whiteMat = make_shared<lambertian>(whiteTex);
	shared_ptr<lambertian> greenMat = make_shared<lambertian>(greenTex);
	shared_ptr<diffuse_light> lightMat = make_shared<diffuse_light>(lightTex);
	
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, greenMat));//右边墙，因为是负方向看，所以是左边墙
	objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, redMat));//左边墙，同上，变成右边墙，都是右手坐标系
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, whiteMat));//底部
	objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, whiteMat));//右手坐标系，z轴正方向那面墙，摄像机在z轴负值，看向z轴原点
	objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, whiteMat));//顶部
	objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, lightMat));//灯光

	objects.add(make_shared<box>(vec3(130, 0, 65), vec3(295, 165, 230), whiteMat));
	objects.add(make_shared<box>(vec3(265, 0, 295), vec3(430, 330, 460), whiteMat));

	//return static_cast<hittable_list>(make_shared<bvh_node>(objects));//46s更慢了 100 100 50
	return objects;//33s w100 h100 s50
}

vec3 ray_color(const ray& r, const hittable_list& world,int depth)
{
    if (depth <= 0)
        return vec3(0, 0, 0);

    hit_record rec;//sphere中计算的返回值全在这
    if (world.hit(r, 0.001, infinity, rec))//world中每个物体都会和这条ray判断，在hittable_list会及时更新t_max，以打到最近的hittable
    {
		ray scattered;
		vec3 attenuation;//atten可以理解为这次碰撞的颜色值，最终加入到ray_color的递归结果，所以是物体的albedo        
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))//碰到的物体，发出scattered射线，变暗了多少atten
            return attenuation * ray_color(scattered, world, depth - 1);
		else//金属材质内部射入或自发光材质都scatter返回false。金属内部属于异常ray,而打到光源这条ray截止
		{
			vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v);//自发光材质的颜色
			return emitted;//打到自发光材质，返回亮度。或打到金属内部，返回000
		}
    }

    //通过遍历hittable_list，这条ray没打到world中任何东西，返回背景色，之前有颜色，现在为黑色
    //光线一旦从缺口跑出去，这条光线就是黑色的了，这也说明samples_per_pixel是2000比4000时候黑点要多
    //因为一个像素射出4000条ray时，虽然也有逃逸，但也有打到光源变亮的可能性变大变多，所以黑点会降低
    //可以想，这个像素2000个中有1500个逃逸了，这个像素只有500个有颜色，只有50个打到光源，被平均后逃逸的黑色占主导
    //4000时候逃逸还是有，只不过更精准了
    return vec3(0, 0, 0);

    vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() 
{
    const int image_width = 100;
    const int image_height = 100;
    const int samples_per_pixel = 4000;
    const int max_depth = 200;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);//下面四项是世界坐标
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    hittable_list world;

    const auto aspect = double(image_width) / image_height;
    vec3 lookfrom;
    vec3 lookat;
    vec3 vup(0, 1, 0);//首先用虚拟的Y轴
    double fov;

    switch (2)
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
	case 2:
		world = cornell_box();
		lookfrom = vec3(278, 278, -800);
		lookat = vec3(278, 278, 0);
		fov = 40;
    default:
        break;
    }

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