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

	virtual vec3 value(double u, double v)const
	{
		if (data == nullptr)
			return vec3(1, 1, 0);

		int i = static_cast<int>((u)* nx); // 在这一行的某像素
		int j = static_cast<int>((1 - v) * ny - 0.001);//在这一列的某像素

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

#endif // !TEXTURE_H

