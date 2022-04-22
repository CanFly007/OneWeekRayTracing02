#ifndef TEXTURE_H
#define TEXTURE_H

class texture
{
public:
	virtual vec3 value(double u, double v)const = 0;
};


class image_texture :public texture
{
public:
	unsigned char* data;//在main中通过stbi_load方法返回的图的内容
	int nx, ny;

public:
	image_texture() {}
	image_texture(unsigned char* pixels, int w, int h) :data(pixels), nx(w), ny(h) {}//构造函数中赋值data
	~image_texture()
	{
		delete data;
	}

	//每个像素的uv坐标，获得这张图对应位置纹素[0,1] vec3值
	virtual vec3 value(double u, double v)const
	{
		if (data == nullptr)
			return vec3(1, 1, 0);

		int i = static_cast<int>((u)* nx); // 在这一行的某像素
		int j = static_cast<int>((1 - v) * ny - 0.001);//main中从上到下，v的uv坐标从1开始到0，而j从0开始计算，所以data数据从一张纹理的左上开始

		//clamp一下
		if (i < 0)i = 0;
		if (j < 0)j = 0;
		if (i > nx - 1)i = nx - 1;
		if (j > ny - 1)j = ny - 1;

		auto rr = data[3 * i + 3 * nx * j + 0];
		auto rri = static_cast<int>(data[3 * i + 3 * nx * j + 0]);
		double r = static_cast<int>(data[3 * i + 3 * nx * j + 0]) / 255.0;//3*i表示一个像素有3个占位，3*nx*j表示到这一行，每行3倍nx
		double g = static_cast<int>(data[3 * i + 3 * nx * j + 1]) / 255.0;//除以255表示rgb为[0,1]
		double b = static_cast<int>(data[3 * i + 3 * nx * j + 2]) / 255.0;

		return vec3(r, g, b);
	}
};

class constant_texture :public texture
{
public:
	vec3 color;

public:
	constant_texture() {}
	constant_texture(vec3 c) :color(c) {}

	virtual vec3 value(double u, double v)const
	{
		return color;//不用管uv，返回固定颜色即可，固定颜色在构造这个texture时候已经赋值
	}

};

#endif // !TEXTURE_H

